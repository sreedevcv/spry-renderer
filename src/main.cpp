#include <cstdint>
#include <cstdlib>
#include <print>
#include <vector>

#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DefaultScene.hpp"
#include "Materials.hpp"
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

        for (const auto& [key, value] : spry::materials) {
            materialNames.push_back(key);
        }

        defaultScene.load();

        lightingShader
            .add(RES_PATH "shaders/Lighting.vert", GL_VERTEX_SHADER)
            .add(RES_PATH "shaders/Lighting.frag", GL_FRAGMENT_SHADER)
            .compile();

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

        plane.load(30, 30);
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
    spry::Cuboid cube;
    spry::Plane plane;
    spry::Shader lightingShader;
    // Uniforms
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPosition = glm::vec3(10.0f, 20.0f, 5.0f);
    spry::Material currMaterial = *spry::materials.at("emerald");
    // ImGuiView
    std::vector<const char*> materialNames;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        defaultScene.process(deltaTime);
        defaultScene.draw();

        lightPosition.x = 5.0f + 10.0f * glm::sin(getGlobalTime());
        lightPosition.y = 20 + 10.0f * glm::cos(getGlobalTime());
        lightPosition.z = 5.0f + 10.0f * glm::sin(getGlobalTime());

        auto model = glm::mat4(1.0f);
        auto view = camera.getViewMatrix();
        auto proj = camera.getProjectionMatrix();

        lightingShader.bind();
        lightingShader.setUniformMatrix("model", model);
        lightingShader.setUniformMatrix("view", view);
        lightingShader.setUniformMatrix("proj", proj);
        lightingShader.setUniformVec("lightColor", lightColor);
        lightingShader.setUniformVec("lightPosition", lightPosition);
        lightingShader.setUniformVec("viewPosition", camera.mPosition);

        lightingShader.setUniformVec("material.ambient", currMaterial.ambient);
        lightingShader.setUniformVec("material.diffuse", currMaterial.diffuse);
        lightingShader.setUniformVec("material.specular", currMaterial.specular);
        lightingShader.setUniformFloat("material.shininess", currMaterial.shininess);

        planeTexture.bind(0);
        testSphere->draw();

        model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 1.0f, 5.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        lightingShader.setUniformMatrix("model", model);
        cube.draw();

        model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -5.0f, -5.0f));
        // model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        lightingShader.setUniformMatrix("model", model);
        sphereTexture.bind(0);
        plane.draw();

        // closeWindow();
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

    int sphereWidth = 10;
    int sphereHeight = 10;
    spry::Sphere* testSphere;
    int currentItem = 0;

    void onImguiDebugDraw(float delta) override
    {
        // ImGui::ShowDemoWindow();

        ImGui::PushItemWidth(180.0f);

        ImGui::Text("FPS: %f", 1.0 / delta);
        ImGui::Text("Delta: %fms", delta * 1000);

        ImGui::Separator();
        ImGui::SliderFloat("lightPosition.x", &lightPosition.x, -100.0f, 100.0f);
        ImGui::SliderFloat("lightPosition.y", &lightPosition.y, -100.0f, 100.0f);
        ImGui::SliderFloat("lightPosition.z", &lightPosition.z, -100.0f, 100.0f);
        ImGui::Separator();

        ImGui::ColorEdit3("Light Color", &lightColor.r);
        ImGui::Separator();

        // List showing all materials
        if (ImGui::ListBox("Materials", &currentItem, materialNames.data(), materialNames.size(), 6)) {
            currMaterial = *spry::materials.at(materialNames[currentItem]);
        }
        ImGui::Separator();

        ImGui::Text("Current Material: %s", materialNames[currentItem]);
        ImGui::ColorEdit3("Ambient Color", &currMaterial.ambient.r);
        ImGui::ColorEdit3("Diffuse Color", &currMaterial.diffuse.r);
        ImGui::ColorEdit3("Specular Color", &currMaterial.specular.r);
        ImGui::Separator();

        ImGui::SliderInt("width", &sphereWidth, 0, 100);
        ImGui::SliderInt("height", &sphereHeight, 0, 100);

        if (ImGui::Button("Generate")) {
            delete testSphere;
            testSphere = new spry::Sphere;
            testSphere->load(sphereWidth, sphereHeight);
        }

        ImGui::PopItemWidth();
    }
};

int main()
{
    TestWindow test(1000, 900);
    test.start();

    return 0;
}
