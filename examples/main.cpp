#include <cstdlib>
#include <print>

#include "BlinnPhongRenderer.hpp"
#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DefaultAxes.hpp"
#include "Entity.hpp"
#include "GLFW/glfw3.h"
#include "Plane.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Toggle.hpp"
#include "Window.hpp"

#include "glm/ext/vector_float3.hpp"
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
        camera.setPosition(glm::vec3(4.0f, 1.0f, 0.0f));
        spry::setCulling(true);
        glCullFace(GL_BACK);

        spry::BlinnPhongRenderer::SpotLight spotLight = {
            .innerCutOffAngle = 0.01f,
            .outerCutOffAngle = 0.1f,
            .constant = 1.0f,
            .linear = 0.09f,
            .quadratic = 0.032f,
        };

        sphere.load(30, 30);
        plane.load(30, 30);
        scene.load(&camera);

        root = new spry::Scene { &sphere, "sphere", "turquoise" };
        const float len = 20.0f;
        root->addChild(std::make_unique<spry::Scene>(
            &cube,
            "basewall",
            "obsidian",
            glm::vec3(-len / 2.0f, -5.0f, -len / 2.0f),
            glm::vec3(len, 1.0f, len)));
        root->addChild(std::make_unique<spry::Scene>(
            &cube,
            "sidewall",
            "redPlastic",
            glm::vec3(-len * 1.5f, -5.0f, -len / 2.0f),
            glm::vec3(len * 2.0f, 1.0f, len * 2.0f),
            glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)))));

        renderer.setSpotLight(spotLight);
        renderer.load(&camera, root);
    }

private:
    int mWidth;
    int mHeight;
    spry::Camera camera;
    spry::BlinnPhongRenderer renderer;
    spry::Sphere sphere;
    spry::Cuboid cube;
    spry::Plane plane;
    spry::DefaultAxes scene;
    float updateTime;
    float prevTime;
    spry::Scene* root;

    void onUpdate(float deltaTime) override
    {
        float time = glfwGetTime();
        processInput(deltaTime);

        renderer.process(deltaTime);
        renderer.render();

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
        renderer.debugView(delta);
    }
};

int main()
{
    TestWindow test(1000, 900);
    test.start();

    return 0;
}
