#include "BlinnPhongRenderer.hpp"

#include "Materials.hpp"
#include "ShaderManager.hpp"

#include "Window.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/ext/vector_float4.hpp"
#include "imgui.h"
#include <cstring>
#include <format>

#include <string_view>

spry::BlinnPhongRenderer::BlinnPhongRenderer()
{
    setCulling(true);
    setDepthTest(true);
    setBlending(false);
    setWireFrameMode(false);
}

void spry::BlinnPhongRenderer::load(const Camera* camera)
{
    // auto s = materials.size();

    // const auto& map = materials;
    // auto c = map.contains({"obsidian"});
    // mCurrMaterial = *map.at({"obsidian"});
    // auto f = false;
    // for (const auto& [key, val] : materials) {
    //     if (key == std::string_view {"emerald"}) {
    //         mCurrMaterial = *val;
    //         f = true;
    //         break;
    //     }
    // }

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

    mShadowMap
        .create()
        .setWrapMode(GL_CLAMP_TO_BORDER)
        .setFilterMode(GL_LINEAR)
        .setBorderColor(borderColors)
        .load(nullptr, mShadowMapWidth, mShadowMapHeight, GL_DEPTH_COMPONENT, GL_FLOAT);

    mShadowMapTarget.load();
    mShadowMapTarget.attachTextureDepth(mShadowMap);

    mDefaultScene.load(camera);
    mTextureViewer.load(glm::vec4(10, 10, 100, 100), mCamera->mScreenWidth, mCamera->mScreenHeight);
    mSphere.load(30, 30);
    mPlane.load(30, 30);
    spry::ShaderManager::instance().loadAndGet(spry::ShaderManager::SHAPE);
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
    // glm::vec3 lightPosition = glm::vec3(10.0f, 20.0f, 5.0f); //

    // mDirLight.direction.x = lightPosition.x + 10.0f * glm::sin(getGlobalTime());
    // // dirLight.direction.y = lightPosition.y + 10.0f * glm::cos(getGlobalTime());
    // dirLight.direction.z = lightPosition.z + 10.0f * glm::sin(getGlobalTime());
}

void spry::BlinnPhongRenderer::render() const
{
    // First pass
    glm::mat4 lightViewProj;

    mShadowMapTarget.bind();
    {
        glViewport(0, 0, mShadowMapWidth, mShadowMapHeight);

        glClear(GL_DEPTH_BUFFER_BIT);

        auto lightProj = glm::ortho(
            -10.0f,
            10.0f,
            -10.0f,
            10.0f,
            0.1f,
            100.0f);
        auto lighView = glm::lookAt(
            mDirLight.direction,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        lightViewProj = lightProj * lighView;

        mShadowPassShader.bind();
        mShadowPassShader.setUniformMatrix("lightViewProj", lightViewProj);

        auto model = glm::mat4(1.0f);
        mShadowPassShader.setUniformMatrix("model", model);
        mSphere.draw();

        model = glm::translate(model, glm::vec3(5.0f, 1.0f, 5.0f));
        model = glm::rotate(model, (float)getGlobalTime(), glm::vec3(1.0f, 1.0f, 0.0f));
        mShadowPassShader.setUniformMatrix("model", model);
        mCuboid.draw();

        mShadowPassShader.setUniformMatrix("model", cubeTwo.getModel());
        mCuboid.draw();

        model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, -5.0f, -15.0f));
        mShadowPassShader.setUniformMatrix("model", model);
        mPlane.draw();
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
    mSphere.draw();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 1.0f, 5.0f));
    model = glm::rotate(model, (float)getGlobalTime(), glm::vec3(1.0f, 1.0f, 0.0f));
    mLightingPassShader.setUniformMatrix("model", model);
    mCuboid.draw();

    mLightingPassShader.setUniformMatrix("model", cubeTwo.getModel());
    mCuboid.draw();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, -5.0f, -15.0f));
    mLightingPassShader.setUniformMatrix("model", model);
    mPlane.draw();
}

void spry::BlinnPhongRenderer::debugView(float delta)
{
    // ImGui::ShowDemoWindow();

    ImGui::PushItemWidth(180.0f);

    ImGui::Text("FPS: %f", 1.0 / delta);
    ImGui::Text("Delta: %.2fms", delta * 1000);
    ImGui::Separator();

    // if (ImGui::SliderInt("X", &tx, 0, mWidth)) {
    //     textureViewer.update(glm::ivec4(tx, ty, 100, 100), mWidth, mHeight);
    // }
    // if (ImGui::SliderInt("Y", &ty, 0, mHeight)) {
    //     textureViewer.update(glm::ivec4(tx, ty, 100, 100), mWidth, mHeight);
    // }

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

    ImGui::SliderFloat("Cube Position.x", &cubeTwo.mPosition.x, -100.0f, 100.0f);
    ImGui::SliderFloat("Cube Position.y", &cubeTwo.mPosition.y, -100.0f, 100.0f);
    ImGui::SliderFloat("Cube Position.z", &cubeTwo.mPosition.z, -100.0f, 100.0f);
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