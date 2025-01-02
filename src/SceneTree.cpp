#include <cstdlib>
#include <print>

#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DefaultScene.hpp"
#include "Plane.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"
#include "Toggle.hpp"
#include "Window.hpp"

#include "imgui.h"

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : spry::Window(width, height, "Test", true)
        , mWidth { width }
        , mHeight { height }
        , camera(width, height)
    {
        setMouseCapture(true);
        camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        uint8_t colors[] = {
            255, 0, 255, 255, //
            255, 0, 0, 255, //
            0, 255, 255, 255, //
            255, 255, 0, 255, //
        };
        texture.create()
            .setWrapMode(GL_REPEAT)
            .setFilterMode(GL_NEAREST)
            .load(colors, 2, 2, GL_RGBA);

        spry::ShaderManager::instance().loadAndGet(spry::ShaderManager::SHAPE);

        sphere.load(30, 30);
        plane.load(30, 30);
        scene.load(&camera);

        planeScene.mEntity.mPosition = glm::vec3(-15.0f, 0.0f, -15.0f);
        planeScene.mEntity.drawable = &plane;

        auto cubeScene = new spry::Scene();
        cubeScene->mEntity.drawable = &cube;
        cubeScene->mEntity.mScale.y = 2.0f;
        planeScene.addChild(std::unique_ptr<spry::Scene> { cubeScene });

        auto sphereScene = new spry::Scene();
        sphereScene->mEntity.drawable = &sphere;
        cubeScene->addChild(std::unique_ptr<spry::Scene> { sphereScene });
    }

private:
    int mWidth;
    int mHeight;
    spry::Camera camera;
    spry::Texture texture;
    spry::Sphere sphere;
    spry::Cuboid cube;
    spry::Plane plane;
    spry::DefaultScene scene;
    spry::Scene planeScene;
    float updateTime;
    float prevTime;

    void onUpdate(float deltaTime) override
    {
        float time = glfwGetTime();
        processInput(deltaTime);

        planeScene.mEntity.mVelocity.x = 3 * glm::sin(spry::getGlobalTime());
        planeScene.getChild(0)->mEntity.mVelocity.z = 5 * glm::sin(2.0 * spry::getGlobalTime());
        planeScene.getChild(0)->mEntity.mPosition.y = 7 * glm::abs(glm::sin(2.0 * spry::getGlobalTime()));

        planeScene.getChild(0)->getChild(0)->mEntity.mPosition.x = 3.0f * glm::sin(spry::getGlobalTime());
        planeScene.getChild(0)->getChild(0)->mEntity.mPosition.z = 3.0f * glm::cos(spry::getGlobalTime());
        planeScene.process(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        const auto& shader = spry::ShaderManager::instance().get(spry::ShaderManager::SHAPE);
        const auto& view = camera.getViewMatrix();
        const auto& proj = camera.getProjectionMatrix();
        const auto model = glm::mat4(1.0f);

        scene.draw();
        shader.bind();
        shader.setUniformMatrix("view", view);
        shader.setUniformMatrix("proj", proj);
        texture.bind(0);
        planeScene.draw(model, shader);

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

    void onImguiDebugDraw(float delta) override
    {
        ImGui::Text("Update Time: %.2f", updateTime);
        const auto& position = planeScene.getChild(0)->getChild(0)->mEntity.mPosition;
        ImGui::Text("Sphere pos: %.2f %.2f %.2f", position.x, position.y, position.z);
    }
};

int main()
{
    TestWindow test(1000, 900);
    test.start();

    return 0;
}
