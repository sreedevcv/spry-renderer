#include <cstdlib>
#include <print>

#include "BlinnPhongRenderer.hpp"
#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DefaultAxes.hpp"
#include "Entity.hpp"
#include "GLFW/glfw3.h"
#include "ImGuiViews.hpp"
#include "Model.hpp"
#include "Plane.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"
#include "Toggle.hpp"
#include "Window.hpp"

#include "glm/ext/matrix_float4x4.hpp"
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
        camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        spry::setCulling(true);
        glCullFace(GL_BACK);
        // for (const auto& [key, value] : spry::materials) {
        //     materialNames.push_back(key);
        // }

        spry::BlinnPhongRenderer::PointLight pointLights[POINT_LIGHT_COUNT];

        pointLights[0] = {
            .position = glm::vec3(7.0f, 2.0f, 2.0f),
            .constant = 1.0,
            .linear = 0.7,
            .quadratic = 1.8,

            .ambient = glm::vec3(0.1f, 0.03f, 0.05f),
            .diffuse = glm::vec3(0.7f, 0.4f, 0.4f),
            .specular = glm::vec3(0.9f, 0.7f, 0.65f),
        };
        pointLights[1] = {
            .position = glm::vec3(5.0f, -4.0f, 10.0f),
            .constant = 1.0,
            .linear = 0.14,
            .quadratic = 0.07,

            .ambient = glm::vec3(0.05f, 0.1f, 0.03f),
            .diffuse = glm::vec3(0.58f, 0.55f, 0.75f),
            .specular = glm::vec3(0.76f, 0.67f, 0.87f),
        };
        pointLights[2] = {
            .position = glm::vec3(15.0f, 12.0f, -12.0f),
            .constant = 1.0,
            .linear = 0.07,
            .quadratic = 0.017,

            .ambient = glm::vec3(0.03f, 0.1f, 0.05f),
            .diffuse = glm::vec3(0.6f, 0.7f, 0.65f),
            .specular = glm::vec3(0.78f, 0.9f, 0.84f),
        };
        pointLights[3] = {
            .position = glm::vec3(0.0f, 0.0f, 30.0f),
            .constant = 1.0,
            .linear = 0.045,
            .quadratic = 0.0075,

            .ambient = glm::vec3(0.15f, 0.15f, 0.11f),
            .diffuse = glm::vec3(0.56f, 0.56f, 0.77f),
            .specular = glm::vec3(0.67f, 0.88f, 0.77f),
        };

        spry::BlinnPhongRenderer::SpotLight spotLight = {
            .innerCutOffAngle = 0.01f,
            .outerCutOffAngle = 0.1f,
            .constant = 1.0f,
            .linear = 0.09f,
            .quadratic = 0.032f,
        };

        renderer.setSpotLight(spotLight);
        for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
            renderer.addPointLight(pointLights[i]);
        }

        renderer.load(&camera);

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

        // spry::ShaderManager::instance().loadAndGet(spry::ShaderManager::SHAPE);

        sphere.load(30, 30);
        plane.load(30, 30);
        scene.load(&camera);

        root.drawable = new spry::Drawable();

        auto sen = new spry::Scene();
        sen->drawable = &sphere;
        auto cen = new spry::Scene();
        cen->mPosition.x = 2.5f;
        cen->mPosition.z = 2.5f;
        cen->drawable = &cube;

        root.addChild(std::unique_ptr<spry::Scene> { sen });
        root.addChild(std::unique_ptr<spry::Scene> { cen });

        mDirLight.direction.x = 0.0f;
        mDirLight.direction.y = 0.0f;
        mDirLight.direction.z = -20.0f;
    }

private:
    int mWidth;
    int mHeight;
    spry::Camera camera;
    spry::BlinnPhongRenderer renderer;
    spry::Texture texture;
    spry::Sphere sphere;
    spry::Cuboid cube;
    spry::Plane plane;
    spry::DefaultAxes scene;
    float updateTime;
    float prevTime;
    spry::BlinnPhongRenderer::DirLight mDirLight;
    spry::Material mCurrMaterial = *spry::materials.at({ "gold" });
    spry::Scene root;

    // ImGuiView
    // std::vector<const char*> materialNames;

    void onUpdate(float deltaTime) override
    {
        float time = glfwGetTime();
        processInput(deltaTime);

        renderer.process(deltaTime);
        renderer.render();


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
        // ImGui::ShowDemoWindow();
        // spry::dbg::viewTransform(transform, "test");

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
