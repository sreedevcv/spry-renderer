#include "BlinnPhongRenderer.hpp"

#include "CubeMap.hpp"
#include "DebugTextureViewer.hpp"
#include "ImGuiViews.hpp"
#include "Materials.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"

#include "Window.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "imgui.h"

#include <cfloat>
#include <cstdint>
#include <cstring>
#include <format>
#include <memory>

spry::BlinnPhongRenderer::BlinnPhongRenderer()
    : mPointLight {
        mShadowMapWidth,
        mShadowMapHeight,
        mFarPlane,
        glm::vec3(3.0f, 5.0f, 2.0f),
        1.0,
        0.0,
        0.0,
        glm::vec3(0.1f, 0.03f, 0.05f),
        glm::vec3(0.7f, 0.4f, 0.4f),
        glm::vec3(0.9f, 0.7f, 0.65f),
    }
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

    mPerpectiveShadowShader
        .add(RES_PATH "shaders/Shape.vert", GL_VERTEX_SHADER)
        .add(RES_PATH "shaders/PerspectiveProjection.frag", GL_FRAGMENT_SHADER)
        .compile();

    mTextureViewer = new DebugTextureViewer(mPerpectiveShadowShader);

    uint8_t colors[] = {
        255, 0, 255, 255, //
        255, 0, 0, 255, //
        0, 255, 255, 255, //
        255, 255, 0, 255, //
    };

    mShapeTexture.create()
        .setWrapMode(GL_REPEAT)
        .setFilterMode(GL_NEAREST)
        .load(colors, 2, 2, GL_RGBA);

    mPointLight.init();
    mDirLight.init(
        glm::vec3(-42.0f, -20.0f, -20.0f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(0.9f, 0.9f, 0.9f),
        mShadowMapWidth,
        mShadowMapHeight);

    mDefaultScene.load(camera);
    mTextureViewer->load(glm::vec4(10, 10, 100, 100), mCamera->mScreenWidth, mCamera->mScreenHeight);
    mSphere.load(30, 30);
    mPlane.load(60, 60);
    mCubeModel.load(RES_PATH "models/cube.obj");

    mSphereScene = new Scene { &mSphere, "sphere", "turquoise" };
    // mSphereScene->addChild(std::make_unique<Scene>(
    //     &mCubeModel,
    //     "cube1",
    //     "obsidian",
    //     glm::vec3(3.0f, -5.0f, -5.0f)));
    // mSphereScene->addChild(std::make_unique<Scene>(
    //     &mCuboid,
    //     "cube2",
    //     "redPlastic",
    //     glm::vec3(5.0f, 1.0f, 5.0f)));
    // mSphereScene->addChild(std::make_unique<Scene>(
    //     &mPlane,
    //     "plane",
    //     "whiteRubber",
    //     glm::vec3(-30.0f, -5.0f, -30.0f),
    //     glm::vec3(60.0f, 2.0f, 60.0f)));
    const float len = 20.0f;
    mSphereScene->addChild(std::make_unique<Scene>(
        &mCubeModel,
        "basewall",
        "obsidian",
        glm::vec3(-len / 2.0f, -5.0f, -len / 2.0f),
        glm::vec3(len, 1.0f, len)));
    mSphereScene->addChild(std::make_unique<Scene>(
        &mCuboid,
        "sidewall",
        "redPlastic",
        glm::vec3(-len * 1.5f, -5.0f, -len / 2.0f),
        glm::vec3(len * 2.0f, 1.0f, len * 2.0f),
        glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)))));
    // mSphereScene->addChild(std::make_unique<Scene>(
    //     &mPlane,
    //     "plane",
    //     "whiteRubber",
    //     glm::vec3(-30.0f, -5.0f, -30.0f),
    //     glm::vec3(60.0f, 2.0f, 60.0f)));
}

void spry::BlinnPhongRenderer::setSpotLight(const SpotLight& spotLight)
{
    mSpotLight = spotLight;
}

void spry::BlinnPhongRenderer::process(float delta)
{
    mDefaultScene.process(delta);
}

// NOTE::Try changing the plane to a cuboid and see whetheer it removes peter panning
void spry::BlinnPhongRenderer::render() const
{
    // First pass

    const auto lightViewProj = mDirLight.renderShadows(mSphereScene);
    mPointLight.renderShadows(mSphereScene);

    glViewport(0, 0, mCamera->mScreenWidth, mCamera->mScreenHeight);
    glClearColor(0.4f, 0.5f, 0.37f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    mDefaultScene.draw();
    // mPerpectiveShadowShader.bind();
    // mPerpectiveShadowShader.setUniformFloat("nearPlane", 0.1F);
    // mPerpectiveShadowShader.setUniformFloat("farPlane", 100.0F);
    // mTextureViewer->draw(mPointLightShadowMap);

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
    mLightingPassShader.setUniformFloat("farPlane", mFarPlane);
    // Options
    mLightingPassShader.setUniformInt("useDirectionalLights", mUseDirectionalLights);
    mLightingPassShader.setUniformInt("useBlinnPhongModel", mUseBlinnPhongModel);
    mLightingPassShader.setUniformInt("usePointLights", mUsePointLights);
    mLightingPassShader.setUniformInt("useSpotLights", mUseSpotLights);
    mLightingPassShader.setUniformInt("shadowSampling", mShadowSampling);
    // SpotLight
    mLightingPassShader.setUniformVec("spotLight.position", mCamera->mPosition);
    mLightingPassShader.setUniformVec("spotLight.direction", mCamera->mFront);
    mLightingPassShader.setUniformFloat("spotLight.innerCutOffAngle", mSpotLight.innerCutOffAngle);
    mLightingPassShader.setUniformFloat("spotLight.outerCutOffAngle", mSpotLight.outerCutOffAngle);
    mLightingPassShader.setUniformFloat("spotLight.constant", mSpotLight.constant);
    mLightingPassShader.setUniformFloat("spotLight.linear", mSpotLight.linear);
    mLightingPassShader.setUniformFloat("spotLight.quadratic", mSpotLight.quadratic);
    // DirLight
    mLightingPassShader.setUniformVec("dirLight.direction", mDirLight.mDirection);
    mLightingPassShader.setUniformVec("dirLight.ambient", mDirLight.mAmbient);
    mLightingPassShader.setUniformVec("dirLight.diffuse", mDirLight.mDiffuse);
    mLightingPassShader.setUniformVec("dirLight.specular", mDirLight.mSpecular);
    // PointLight
    for (int i = 0; i < POINT_LIGHT_COUNT; i++) {

        mPointLight.bindUniforms(mLightingPassShader, i);
        auto model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::translate(model, mPointLight.mPosition);

        shapeShader.bind();
        shapeShader.setUniformMatrix("model", model);
        shapeShader.setUniformMatrix("view", view);
        shapeShader.setUniformMatrix("proj", proj);
        mShapeTexture.bind(0);
        mCuboid.draw();
    }

    constexpr auto materialDrawCallback = [](const Scene* scene, const Shader& shader) {
        shader.setUniformVec("material.ambient", scene->mMaterial.ambient);
        shader.setUniformVec("material.diffuse", scene->mMaterial.diffuse);
        shader.setUniformVec("material.specular", scene->mMaterial.specular);
        shader.setUniformFloat("material.shininess", scene->mMaterial.shininess);
    };

    mLightingPassShader.bind();
    mDirLight.getDepthMap().bind(0);
    mPointLight.getCubeMap().bind(1);
    mLightingPassShader.setUniformInt("dirLightShadowMap", 0);
    mLightingPassShader.setUniformInt("pointLightShadowMap", 1);
    mSphereScene->draw(model, mLightingPassShader, materialDrawCallback);
}

void spry::BlinnPhongRenderer::debugView(float delta)
{
    ImGui::PushItemWidth(180.0f);

    ImGui::Text("FPS: %f", 1.0 / delta);
    ImGui::Text("Delta: %.2fms", delta * 1000);
    ImGui::Separator();
    ImGui::Text("Light to origin: %0.2f", glm::length(mPointLight.mPosition));
    dbg::viewPointLight(mPointLight);
    ImGui::Separator();

    dbg::viewSceneTree(mSphereScene);

    ImGui::Separator();

    ImGui::SliderInt("Shdw sampling", &mShadowSampling, 0, 10);

    const auto camNorm = glm::normalize(mCamera->mFront);
    const auto dirNorm = glm::normalize(mDirLight.mDirection);
    ImGui::Text("Camera pos:  %.2f  %.2f  %.2f", mCamera->mPosition.x, mCamera->mPosition.y, mCamera->mPosition.z);
    ImGui::Text("Camera dir:  %.2f  %.2f  %.2f", camNorm.x, camNorm.y, camNorm.z);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Lights Option")) {
        if (ImGui::Button("align light dir")) {
            mDirLight.mDirection = mCamera->mFront;
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

    ImGui::SliderFloat("DirLight Direction.x", &mDirLight.mDirection.x, -100.0f, 100.0f);
    ImGui::SliderFloat("DirLight Direction.y", &mDirLight.mDirection.y, -100.0f, 100.0f);
    ImGui::SliderFloat("DirLight Direction.z", &mDirLight.mDirection.z, -100.0f, 100.0f);
    ImGui::Separator();

    ImGui::ColorEdit3("DirLight Ambient Color", &mDirLight.mAmbient.r);
    ImGui::ColorEdit3("DirLight Diffuse Color", &mDirLight.mDiffuse.r);
    ImGui::ColorEdit3("DirLight Specular Color", &mDirLight.mSpecular.r);
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