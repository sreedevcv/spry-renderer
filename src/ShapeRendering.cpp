#include <cstdint>
#include <cstdlib>
#include <print>

#include "Camera.hpp"
#include "DefaultScene.hpp"
#include "Plane.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"
#include "Toggle.hpp"
#include "Window.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include "imgui.h"

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : spry::Window(width, height, "Test", true)
        , mWidth { width }
        , mHeight { height }
        , camera(width, height)
        , defaultScene(camera)
    {
        setCulling(false);
        setDepthTest(true);
        setBlending(true);
        setMouseCapture(false);
        setWireFrameMode(false);
        camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        spry::ShaderManager::instance().loadAndGet(spry::ShaderManager::SHAPE);

        defaultScene.load();

        uint8_t color1[4] = { 255, 0, 0, 255 };

        planeTexture
            .create()
            .setWrapMode(GL_CLAMP_TO_EDGE)
            .setFilterMode(GL_LINEAR)
            .load(color1, 1, 1, GL_RGBA);

        uint8_t color2[] = { 0, 255, 0, 255 };

        sphereTexture
            .create()
            .setWrapMode(GL_CLAMP_TO_EDGE)
            .setFilterMode(GL_LINEAR)
            .load(color2, 1, 1, GL_RGBA);

        // .load(RES_PATH; "models/Sponza-master/textures/sponza_fabric_blue_diff.tga");

        plane.load(10, 10);
        testSphere = new spry::Sphere;
        testSphere->load(sphereWidth, sphereHeight);
    }

private:
    int mWidth;
    int mHeight;
    spry::Camera camera;
    spry::DefaultScene defaultScene;
    spry::Texture planeTexture;
    spry::Texture sphereTexture;
    spry::Sphere sphere;
    spry::Plane plane;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);
        const auto& shapeShader = spry::ShaderManager::instance().get(spry::ShaderManager::SHAPE);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        defaultScene.process(deltaTime);
        defaultScene.draw();

        auto model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)getGlobalTime(), glm::vec3(1.0f, 0.0f, 1.0f));
        auto view = camera.getViewMatrix();
        auto proj = camera.getProjectionMatrix();

        shapeShader.bind();
        shapeShader.setUniformMatrix("model", model);
        shapeShader.setUniformMatrix("view", view);
        shapeShader.setUniformMatrix("proj", proj);

        planeTexture.bind(0);
        testSphere->draw();

        model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 5.0f, -10.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shapeShader.setUniformMatrix("model", model);
        sphereTexture.bind(0);
        plane.draw();
    }

    spry::Toggle toggle;

    void processInput(float deltaTime)
    {
        toggle.update(deltaTime);

        if (isKeyPressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
        if (isKeyPressed(GLFW_KEY_ENTER) && toggle.canToggle()) {
            setWireFrameMode(toggle.toggle());
        }

        camera.processInputDefault(*this, deltaTime);
    }

    void onMouseMove(double xPosIn, double yPosIn) override
    {
        camera.onMouseMoveDefault(xPosIn, yPosIn);
    }

    void onMouseScroll(double xOffset, double yOffset) override
    {
        camera.onMouseScrollDefault(yOffset);
    }

    void onScreenSizeChange(int width, int height) override
    {
        camera.setScreenSize(width, height);
    }

    int sphereWidth = 10;
    int sphereHeight = 10;
    spry::Sphere* testSphere;

    void onImguiDebugDraw(float delta) override
    {
        ImGui::Text("FPS: %f", 1.0 / delta);
        ImGui::Text("Delta: %fms", delta * 1000);

        ImGui::SliderInt("width", &sphereWidth, 0, 100);
        ImGui::SliderInt("height", &sphereHeight, 0, 100);

        if (ImGui::Button("Generate")) {
            delete testSphere;
            testSphere = new spry::Sphere;
            testSphere->load(sphereWidth, sphereHeight);
        }
    }
};

int main()
{
    TestWindow test(800, 600);
    test.start();

    return 0;
}
