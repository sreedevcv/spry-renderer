#include <cstdint>
#include <cstdlib>
#include <format>
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
#include "spdlog/spdlog.h"

#define POINT_LIGHT_COUNT 4

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
        spry::ShaderManager::instance().loadAndGet(spry::ShaderManager::SHAPE);

        lightingShader
            .add(RES_PATH "shaders/Lighting.vert", GL_VERTEX_SHADER)
            .add(RES_PATH "shaders/Lighting.frag", GL_FRAGMENT_SHADER)
            .compile();

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

        spotLight = {
            .innerCutOffAngle = 0.01f,
            .outerCutOffAngle = 0.1f,
            .constant = 1.0f,
            .linear = 0.09f,
            .quadratic = 0.032f,
        };

        uint8_t color1[4]
            = { 255, 0, 0, 255 };

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
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //
    glm::vec3 lightPosition = glm::vec3(10.0f, 20.0f, 5.0f); //
    spry::Material currMaterial = *spry::materials.at("emerald");

    struct DirLight {
        glm::vec3 direction = glm::vec3(10.0f, 20.0f, 5.0f);

        glm::vec3 ambient = glm::vec3(0.18f, 0.18f, 0.18f);
        glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 specular = glm::vec3(0.9f, 0.9f, 0.9f);
    };

    struct PointLight {
        glm::vec3 position;

        float constant;
        float linear;
        float quadratic;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct SpotLight {
        float innerCutOffAngle;
        float outerCutOffAngle;

        float constant;
        float linear;
        float quadratic;
    };

    DirLight dirLight;
    PointLight pointLights[POINT_LIGHT_COUNT];
    SpotLight spotLight;

    // ImGuiView
    std::vector<const char*>
        materialNames;

    void onUpdate(float deltaTime) override
    {
        const auto& shapeShader = spry::ShaderManager::instance().get(spry::ShaderManager::SHAPE);
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

        planeTexture.bind(0);

        lightingShader.bind();
        lightingShader.setUniformMatrix("model", model);
        lightingShader.setUniformMatrix("view", view);
        lightingShader.setUniformMatrix("proj", proj);
        lightingShader.setUniformVec("viewPosition", camera.mPosition);

        // Materials
        lightingShader.setUniformVec("material.ambient", currMaterial.ambient);
        lightingShader.setUniformVec("material.diffuse", currMaterial.diffuse);
        lightingShader.setUniformVec("material.specular", currMaterial.specular);
        lightingShader.setUniformFloat("material.shininess", currMaterial.shininess);
        // SpotLight
        lightingShader.setUniformVec("spotLight.position", camera.mPosition);
        lightingShader.setUniformVec("spotLight.direction", camera.mFront);
        lightingShader.setUniformFloat("spotLight.innerCutOffAngle", spotLight.innerCutOffAngle);
        lightingShader.setUniformFloat("spotLight.outerCutOffAngle", spotLight.outerCutOffAngle);
        lightingShader.setUniformFloat("spotLight.constant", spotLight.constant);
        lightingShader.setUniformFloat("spotLight.linear", spotLight.linear);
        lightingShader.setUniformFloat("spotLight.quadratic", spotLight.quadratic);

        // DirLight
        lightingShader.setUniformVec("dirLight.direction", dirLight.direction);
        lightingShader.setUniformVec("dirLight.ambient", dirLight.ambient);
        lightingShader.setUniformVec("dirLight.diffuse", dirLight.diffuse);
        lightingShader.setUniformVec("dirLight.specular", dirLight.specular);
        // PointLight
        for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
            glm::vec3 newPos;
            newPos.x = pointLights[i].position.x + 10.0f * glm::sin(getGlobalTime());
            newPos.y = pointLights[i].position.y + 10.0f * glm::cos(getGlobalTime());
            newPos.z = pointLights[i].position.z + 10.0f * glm::sin(getGlobalTime());

            lightingShader.bind();
            lightingShader.setUniformVec(std::format("pointLights[{}].position", i).c_str(), newPos);
            lightingShader.setUniformFloat(std::format("pointLights[{}].constant", i).c_str(), pointLights[i].constant);
            lightingShader.setUniformFloat(std::format("pointLights[{}].linear", i).c_str(), pointLights[i].linear);
            lightingShader.setUniformFloat(std::format("pointLights[{}].quadratic", i).c_str(), pointLights[i].quadratic);
            lightingShader.setUniformVec(std::format("pointLights[{}].ambient", i).c_str(), pointLights[i].ambient);
            lightingShader.setUniformVec(std::format("pointLights[{}].diffuse", i).c_str(), pointLights[i].diffuse);
            lightingShader.setUniformVec(std::format("pointLights[{}].specular", i).c_str(), pointLights[i].specular);

            auto model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
            model = glm::translate(model, newPos);

            shapeShader.bind();
            shapeShader.setUniformMatrix("model", model);
            shapeShader.setUniformMatrix("view", view);
            shapeShader.setUniformMatrix("proj", proj);
            cube.draw();
        }

        lightingShader.bind();
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

        ImGui::SliderFloat("innerCutOffAngle", &spotLight.innerCutOffAngle, 0.0f, glm::pi<float>() / 2);
        ImGui::SliderFloat("outerCutOffAngle", &spotLight.outerCutOffAngle, 0.0f, glm::pi<float>() / 2);

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
        ImGui::SliderFloat("Shininesss", &currMaterial.shininess, 0.0f, 10.0f);
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
