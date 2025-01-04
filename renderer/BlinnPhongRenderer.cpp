#include "BlinnPhongRenderer.hpp"

#include "ImGuiViews.hpp"
#include "Materials.hpp"
#include "Scene.hpp"
#include "ShaderManager.hpp"

#include "Window.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/geometric.hpp"
#include "imgui.h"
#include <cstring>
#include <format>
#include <memory>

spry::BlinnPhongRenderer::BlinnPhongRenderer()
{
    setCulling(true);
    setDepthTest(true);
    setBlending(false);
    setWireFrameMode(false);
}

void spry::BlinnPhongRenderer::load(Camera* camera)
{
    spry::ShaderManager::instance().loadAndGet(spry::ShaderManager::SHAPE);
    mCamera = camera;

    mLightingPassShader
        .add(RES_PATH "shaders/Lighting.vert", GL_VERTEX_SHADER)
        .add(RES_PATH "shaders/Lighting.frag", GL_FRAGMENT_SHADER)
        .compile();

    mShadowPassShader
        .add(RES_PATH "shaders/Shadow.vert", GL_VERTEX_SHADER)
        .add(RES_PATH "shaders/Shadow.frag", GL_FRAGMENT_SHADER)
        .compile();

    std::array borderColors { 1.0f, 1.0f, 1.0f, 1.0f };

    mShadowMapWidth = mCamera->mScreenWidth;
    mShadowMapHeight = mCamera->mScreenHeight;

    mShadowMap
        .create()
        .setWrapMode(GL_CLAMP_TO_BORDER)
        .setFilterMode(GL_NEAREST)
        .setBorderColor(borderColors)
        .load(nullptr, mShadowMapWidth, mShadowMapHeight, GL_DEPTH_COMPONENT, GL_FLOAT);

    mShadowMapTarget.load();
    mShadowMapTarget.attachTextureDepth(mShadowMap);

    mDefaultScene.load(camera);
    mTextureViewer.load(glm::vec4(10, 10, 100, 100), mCamera->mScreenWidth, mCamera->mScreenHeight);
    mSphere.load(30, 30);
    mPlane.load(60, 60);

    mSphereScene = new Scene { &mSphere, "sphere" };
    mSphereScene->addChild(std::make_unique<Scene>(
        &mCuboid,
        "cube1",
        glm::vec3(3.0f, -5.0f, -5.0f)));
    mSphereScene->addChild(std::make_unique<Scene>(
        &mCuboid,
        "cube2",
        glm::vec3(5.0f, 1.0f, 5.0f)));
    mSphereScene->addChild(std::make_unique<Scene>(
        &mPlane,
        "plane",
        glm::vec3(-30.0f, -5.0f, -30.0f),
        glm::vec3(2.0f, 2.0f, 2.0f)));
}

void spry::BlinnPhongRenderer::addPointLight(const PointLight& pointLight)
{
    if (mPointLights.size() < POINT_LIGHT_COUNT) {
        mPointLights.push_back(pointLight);
    }
}

void spry::BlinnPhongRenderer::setDirLight(const DirLight& dirLight)
{
    mDirLight = dirLight;
}

void spry::BlinnPhongRenderer::setSpotLight(const SpotLight& spotLight)
{
    mSpotLight = spotLight;
}

void spry::BlinnPhongRenderer::process(float delta)
{
    mDefaultScene.process(delta);
}

void spry::BlinnPhongRenderer::render() const
{
    // First pass
    glm::mat4 lightViewProj;

    mShadowMapTarget.bind();
    {
        // glCullFace(GL_FRONT);
        glViewport(0, 0, mShadowMapWidth, mShadowMapHeight);
        glClear(GL_DEPTH_BUFFER_BIT);

        auto lightProj = glm::ortho(
            oleft,
            oright,
            obottom,
            otop,
            onear,
            ofar);
        // NOTE::Should eye for ortho projection be -direction or +direction??
        auto lighView = glm::lookAt(
            -mDirLight.direction,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        lightViewProj = lightProj * lighView;

        mShadowPassShader.bind();
        mShadowPassShader.setUniformMatrix("lightViewProj", lightViewProj);

        auto model = glm::mat4(1.0f);
        mSphereScene->draw(model, mShadowPassShader);

        // glCullFace(GL_BACK);

        /* NOTE::A plane has a only a single face. Using
        ** glCullFace(GL_FRONT) would completely rempve it
        ** So it should not be used to render is
        */
    }
    mShadowMapTarget.unbind();

    glViewport(0, 0, mCamera->mScreenWidth, mCamera->mScreenHeight);
    glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    mDefaultScene.draw();
    mTextureViewer.draw(mShadowMap);

    const auto& shapeShader = spry::ShaderManager::instance().get(spry::ShaderManager::SHAPE);
    const auto view = mCamera->getViewMatrix();
    const auto proj = mCamera->getProjectionMatrix();
    auto model = glm::mat4(1.0f);

    mLightingPassShader.bind();
    // Vertex Shader
    mLightingPassShader.setUniformMatrix("model", model);
    mLightingPassShader.setUniformMatrix("view", view);
    mLightingPassShader.setUniformMatrix("proj", proj);
    mLightingPassShader.setUniformMatrix("lightViewProj", lightViewProj);
    // Fragment Shader
    mLightingPassShader.setUniformVec("viewPosition", mCamera->mPosition);
    mLightingPassShader.setUniformInt("useBlinnPhongModel", mUseBlinnPhongModel);
    mLightingPassShader.setUniformInt("useDirectionalLights", mUseDirectionalLights);
    mLightingPassShader.setUniformInt("usePointLights", mUsePointLights);
    mLightingPassShader.setUniformInt("useSpotLights", mUseSpotLights);
    // Materials
    mLightingPassShader.setUniformVec("material.ambient", mCurrMaterial.ambient);
    mLightingPassShader.setUniformVec("material.diffuse", mCurrMaterial.diffuse);
    mLightingPassShader.setUniformVec("material.specular", mCurrMaterial.specular);
    mLightingPassShader.setUniformFloat("material.shininess", mCurrMaterial.shininess);
    // SpotLight
    mLightingPassShader.setUniformVec("spotLight.position", mCamera->mPosition);
    mLightingPassShader.setUniformVec("spotLight.direction", mCamera->mFront);
    mLightingPassShader.setUniformFloat("spotLight.innerCutOffAngle", mSpotLight.innerCutOffAngle);
    mLightingPassShader.setUniformFloat("spotLight.outerCutOffAngle", mSpotLight.outerCutOffAngle);
    mLightingPassShader.setUniformFloat("spotLight.constant", mSpotLight.constant);
    mLightingPassShader.setUniformFloat("spotLight.linear", mSpotLight.linear);
    mLightingPassShader.setUniformFloat("spotLight.quadratic", mSpotLight.quadratic);
    // DirLight
    mLightingPassShader.setUniformVec("dirLight.direction", mDirLight.direction);
    mLightingPassShader.setUniformVec("dirLight.ambient", mDirLight.ambient);
    mLightingPassShader.setUniformVec("dirLight.diffuse", mDirLight.diffuse);
    mLightingPassShader.setUniformVec("dirLight.specular", mDirLight.specular);
    // PointLight
    for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
        glm::vec3 newPos;
        newPos.x = mPointLights[i].position.x + 10.0f * glm::sin(getGlobalTime());
        newPos.y = mPointLights[i].position.y + 10.0f * glm::cos(getGlobalTime());
        newPos.z = mPointLights[i].position.z + 10.0f * glm::sin(getGlobalTime());

        mLightingPassShader.bind();
        mLightingPassShader.setUniformVec(std::format("pointLights[{}].position", i).c_str(), newPos);
        mLightingPassShader.setUniformFloat(std::format("pointLights[{}].constant", i).c_str(), mPointLights[i].constant);
        mLightingPassShader.setUniformFloat(std::format("pointLights[{}].linear", i).c_str(), mPointLights[i].linear);
        mLightingPassShader.setUniformFloat(std::format("pointLights[{}].quadratic", i).c_str(), mPointLights[i].quadratic);
        mLightingPassShader.setUniformVec(std::format("pointLights[{}].ambient", i).c_str(), mPointLights[i].ambient);
        mLightingPassShader.setUniformVec(std::format("pointLights[{}].diffuse", i).c_str(), mPointLights[i].diffuse);
        mLightingPassShader.setUniformVec(std::format("pointLights[{}].specular", i).c_str(), mPointLights[i].specular);

        auto model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::translate(model, newPos);

        shapeShader.bind();
        shapeShader.setUniformMatrix("model", model);
        shapeShader.setUniformMatrix("view", view);
        shapeShader.setUniformMatrix("proj", proj);
        mCuboid.draw();
    }

    mLightingPassShader.bind();
    mShadowMap.bind(0);
    mSphereScene->draw(model, mLightingPassShader);
}

void spry::BlinnPhongRenderer::debugView(float delta)
{
    ImGui::PushItemWidth(180.0f);

    ImGui::Text("FPS: %f", 1.0 / delta);
    ImGui::Text("Delta: %.2fms", delta * 1000);
    ImGui::Separator();

    dbg::viewSceneTree(mSphereScene);

    ImGui::Separator();

    const auto camNorm = glm::normalize(mCamera->mFront);
    const auto dirNorm = glm::normalize(mDirLight.direction);
    ImGui::Text("Camera pos:  %.2f  %.2f  %.2f", mCamera->mPosition.x, mCamera->mPosition.y, mCamera->mPosition.z);
    ImGui::Text("Camera dir:  %.2f  %.2f  %.2f", camNorm.x, camNorm.y, camNorm.z);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Ortho Projection Details")) {
        ImGui::SliderFloat("oleft", &oleft, -300.0f, 300.0f);
        ImGui::SliderFloat("oright", &oright, -300.0f, 300.0f);
        ImGui::SliderFloat("otop", &otop, -300.0f, 300.0f);
        ImGui::SliderFloat("obottom", &obottom, -300.0f, 300.0f);
        ImGui::SliderFloat("onear", &onear, -300.0f, 300.0f);
        ImGui::SliderFloat("ofar", &ofar, -300.0f, 300.0f);
    }

    if (ImGui::CollapsingHeader("Lights Option")) {
        if (ImGui::Button("align light dir")) {
            mDirLight.direction = mCamera->mFront;
        }
        if (ImGui::Button(std::format("Use useBlinnPhongModel[{}]", mUseBlinnPhongModel == 1).c_str())) {
            mUseBlinnPhongModel = (mUseBlinnPhongModel == 0) ? 1 : 0;
        }
        if (ImGui::Button(std::format("Use DirectionalLights[{}]", mUseDirectionalLights == 1).c_str())) {
            mUseDirectionalLights = (mUseDirectionalLights == 0) ? 1 : 0;
        }
        if (ImGui::Button(std::format("Use PointLights[{}]", mUsePointLights == 1).c_str())) {
            mUsePointLights = (mUsePointLights == 0) ? 1 : 0;
        }
        if (ImGui::Button(std::format("Use SpotLights[{}]", mUseSpotLights == 1).c_str())) {
            mUseSpotLights = (mUseSpotLights == 0) ? 1 : 0;
        }
    }

    ImGui::Separator();

    ImGui::SliderFloat("innerCutOffAngle", &mSpotLight.innerCutOffAngle, 0.0f, glm::pi<float>() / 2);
    ImGui::SliderFloat("outerCutOffAngle", &mSpotLight.outerCutOffAngle, 0.0f, glm::pi<float>() / 2);
    ImGui::Separator();

    ImGui::SliderFloat("DirLight Direction.x", &mDirLight.direction.x, -100.0f, 100.0f);
    ImGui::SliderFloat("DirLight Direction.y", &mDirLight.direction.y, -100.0f, 100.0f);
    ImGui::SliderFloat("DirLight Direction.z", &mDirLight.direction.z, -100.0f, 100.0f);
    ImGui::Separator();

    ImGui::ColorEdit3("DirLight Ambient Color", &mDirLight.ambient.r);
    ImGui::ColorEdit3("DirLight Diffuse Color", &mDirLight.diffuse.r);
    ImGui::ColorEdit3("DirLight Specular Color", &mDirLight.specular.r);
    ImGui::Separator();

    // // List showing all materials
    // if (ImGui::ListBox("Materials", &currentItem, materialNames.data(), materialNames.size(), 6)) {
    //     currMaterial = *spry::materials.at(materialNames[currentItem]);
    // }
    // ImGui::Separator();

    // ImGui::Text("Current Material: %s", materialNames[currentItem]);
    // ImGui::ColorEdit3("Ambient Color", &currMaterial.ambient.r);
    // ImGui::ColorEdit3("Diffuse Color", &currMaterial.diffuse.r);
    // ImGui::ColorEdit3("Specular Color", &currMaterial.specular.r);
    // ImGui::SliderFloat("Shininesss", &currMaterial.shininess, 0.0f, 10.0f);
    // ImGui::Separator();

    ImGui::PopItemWidth();
}