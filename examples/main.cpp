#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <print>

#include "BlinnPhongRenderer.hpp"
#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DefaultAxes.hpp"
#include "Entity.hpp"
#include "FullScreenQuad.hpp"
#include "GLFW/glfw3.h"
#include "Plane.hpp"
#include "Shader.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"
#include "TextureRenderTarget.hpp"
#include "Toggle.hpp"
#include "VAO.hpp"
#include "Window.hpp"

#include "glm/ext/vector_float3.hpp"
#include "imgui.h"
#include "spdlog/spdlog.h"

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : spry::Window(width, height, "Test", true)
        , camera(width, height)
    {
        setMouseCapture(true);
        camera.setPosition(glm::vec3(40.0f, 60.0f, 0.0f));
        spry::setCulling(false);
        spry::setDepthTest(true);
        glCullFace(GL_BACK);
        glViewport(0, 0, width, height);

        GLint maxTessLevel;
        glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);
        spdlog::info("Max available tess level: {} ", maxTessLevel);
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        heightMap
            .create()
            .setWrapMode(GL_REPEAT)
            .setFilterMode(GL_LINEAR)
            .load(RES_PATH "iceland_heightmap.png");

        constexpr uint32_t resolution = 20;
        std::vector<float> vertices;
        int32_t imgHeight = heightMap.mHeight;
        int32_t imgWidth = heightMap.mWidth;

        for (uint32_t i = 0; i < resolution; i++) {
            for (uint32_t j = 0; j < resolution; j++) {
                vertices.push_back(-imgWidth / 2.0f + imgWidth * i / float(resolution));
                vertices.push_back(0.0f);
                vertices.push_back(-imgHeight / 2.0f + imgHeight * j / float(resolution));
                vertices.push_back(i / float(resolution));
                vertices.push_back(j / float(resolution));

                vertices.push_back(-imgWidth / 2.0f + imgWidth * (i + 1) / float(resolution));
                vertices.push_back(0.0f);
                vertices.push_back(-imgHeight / 2.0f + imgHeight * j / float(resolution));
                vertices.push_back((i + 1) / float(resolution));
                vertices.push_back(j / float(resolution));

                vertices.push_back(-imgWidth / 2.0f + imgWidth * i / float(resolution));
                vertices.push_back(0.0f);
                vertices.push_back(-imgHeight / 2.0f + imgHeight * (j + 1) / float(resolution));
                vertices.push_back(i / float(resolution));
                vertices.push_back((j + 1) / float(resolution));

                vertices.push_back(-imgWidth / 2.0f + imgWidth * (i + 1) / float(resolution));
                vertices.push_back(0.0f);
                vertices.push_back(-imgHeight / 2.0f + imgHeight * (j + 1) / float(resolution));
                vertices.push_back((i + 1) / float(resolution));
                vertices.push_back((j + 1) / float(resolution));
            }
        }

        std::array<uint32_t, 2> format { 3, 2 };
        mVAO.load(vertices, format, resolution * resolution * 4, GL_STATIC_DRAW);
        tesselationShader
            .add(RES_PATH "shaders/HeightMap.vert", GL_VERTEX_SHADER)
            .add(RES_PATH "shaders/HeightMap.frag", GL_FRAGMENT_SHADER)
            .add(RES_PATH "shaders/HeightMap.tessctl.glsl", GL_TESS_CONTROL_SHADER)
            .add(RES_PATH "shaders/HeightMap.tesseval.glsl", GL_TESS_EVALUATION_SHADER)
            .compile();

        sphere.load(30, 30);
        plane.load(30, 30);
        scene.load(&camera);
        // renderer.load(&camera, root);

        spry::setWireFrameMode(true);

        texture
            .create()
            .setFilterMode(GL_LINEAR)
            .setWrapMode(GL_CLAMP_TO_BORDER)
            .load(nullptr, width, height, GL_RGBA);

        quad.laod();
        target.load();
        target.attachTextureColor(texture);

    }

private:
    spry::Camera camera;
    // spry::BlinnPhongRenderer renderer;
    spry::Sphere sphere;
    spry::Cuboid cube;
    spry::Plane plane;
    spry::DefaultAxes scene;
    float updateTime;
    float prevTime;
    // spry::Scene* root;
    spry::VAO mVAO;
    spry::Shader tesselationShader;
    spry::Texture heightMap;
    spry::TextureRenderTarget target;
    spry::Texture texture;
    spry::FullScreenQuad quad;

    void onUpdate(float deltaTime) override
    {
        float time = glfwGetTime();
        processInput(deltaTime);

        // renderer.process(deltaTime);
        // renderer.render();

        const auto model = glm::mat4 { 1.0f };
        // target.bind();
        {
            glClearColor(0.2f, 0.2f, 0.24f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene.draw();

            tesselationShader.bind();
            tesselationShader.setUniformMatrix("proj", camera.getProjectionMatrix());
            tesselationShader.setUniformMatrix("view", camera.getViewMatrix());
            tesselationShader.setUniformMatrix("model", model);
            tesselationShader.setUniformInt("heightMap", 0);

            heightMap.bind(0);
            mVAO.draw(GL_PATCHES);
        }
        // target.unbind();

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // texture.bind(0);
        // quad.draw();

        // closeWindow();

        updateTime = time - prevTime;
        prevTime = time;
    }

    spry::Toggle toggle;
    spry::Toggle debugToggle;

    void processInput(float deltaTime)
    {
        toggle.update(deltaTime);
        debugToggle.update(deltaTime);

        if (isKeyPressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
        if (isKeyPressed(GLFW_KEY_ENTER) && debugToggle.canToggle()) {
            setMouseCapture(debugToggle.toggle());
        }

        camera.processInputDefault(*this, deltaTime);
    }

    void onMouseMove(double xPosIn, double yPosIn) override
    {
        if (debugToggle.getValue()) {
            camera.onMouseMoveDefault(xPosIn, yPosIn);
        }
    }

    void onMouseScroll(double xOffset, double yOffset) override
    {
        if (debugToggle.getValue()) {
            camera.onMouseScrollDefault(yOffset);
        }
    }

    void onScreenSizeChange(int width, int height) override
    {
        camera.setScreenSize(width, height);
    }

    spry::Transform transform;
    void onImguiDebugDraw(float delta) override
    {
        ImGui::Text("Update Time: %.2f", updateTime);
        ImGui::Separator();
        // renderer.debugView(delta);
    }
};

int main()
{
    TestWindow test(1000, 900);
    test.start();

    return 0;
}
