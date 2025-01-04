#include <cstdlib>
#include <print>

#include "BlinnPhongRenderer.hpp"
#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DefaultScene.hpp"
#include "Entity.hpp"
#include "GLFW/glfw3.h"
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

        // renderer.setSpotLight(spotLight);
        // for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
        //     renderer.addPointLight(pointLights[i]);
        // }

        // renderer.load(&camera);

        // uint8_t colors[] = {
        //     255, 0, 255, 255, //
        //     255, 0, 0, 255, //
        //     0, 255, 255, 255, //
        //     255, 255, 0, 255, //
        // };

        // texture.create()
        //     .setWrapMode(GL_REPEAT)
        //     .setFilterMode(GL_NEAREST)
        //     .load(colors, 2, 2, GL_RGBA);

        // spry::ShaderManager::instance().loadAndGet(spry::ShaderManager::SHAPE);

        sphere.load(30, 30);
        plane.load(30, 30);
        // scene.load(&camera);

        // planeScene.mEntity.mPosition = glm::vec3(-15.0f, 0.0f, -15.0f);
        // planeScene.mEntity.drawable = &plane;

        // auto cubeScene = new spry::Scene();
        // cubeScene->mEntity.drawable = &cube;
        // cubeScene->mEntity.mScale.y = 2.0f;
        // planeScene.addChild(std::unique_ptr<spry::Scene> { cubeScene });

        // auto sphereScene = new spry::Scene();
        // sphereScene->mEntity.drawable = &sphere;
        // cubeScene->addChild(std::unique_ptr<spry::Scene> { sphereScene });

        simpleShader
            .add(RES_PATH "shaders/Test.vert", GL_VERTEX_SHADER)
            .add(RES_PATH "shaders/Test.frag", GL_FRAGMENT_SHADER)
            .compile();

        scene.load(&camera);

        sphereModel.load(RES_PATH "models/sphere.obj");
        cubeModel.load(RES_PATH "models/cube.obj");

        root.drawable = new spry::Drawable();

        auto sen = new spry::Scene();
        sen->drawable = &sphere;
        auto cen = new spry::Scene();
        cen->mPosition.x = 2.5f;
        cen->mPosition.z = 2.5f;
        cen->drawable = &cube;
        auto bcen = new spry::Scene();
        bcen->mPosition.x = 2.0f * 3.0;
        bcen->mPosition.z = 2.0f * 3.0;
        bcen->drawable = &cubeModel;
        auto bsen = new spry::Scene();
        bsen->mPosition.x = 2.0f * 5.0;
        bsen->mPosition.z = 2.0f * 5.0;
        bsen->drawable = &sphereModel;

        root.addChild(std::unique_ptr<spry::Scene> { sen });
        root.addChild(std::unique_ptr<spry::Scene> { cen });
        root.addChild(std::unique_ptr<spry::Scene> { bcen });
        root.addChild(std::unique_ptr<spry::Scene> { bsen });

        mDirLight.direction.x = 0.0f;
        mDirLight.direction.y = 0.0f;
        mDirLight.direction.z = -20.0f;
    }

private:
    int mWidth;
    int mHeight;
    spry::Camera camera;
    // spry::BlinnPhongRenderer renderer;
    spry::Texture texture;
    spry::Sphere sphere;
    spry::Cuboid cube;
    spry::Plane plane;
    spry::Shader simpleShader;
    spry::DefaultScene scene;
    float updateTime;
    float prevTime;
    spry::Model sphereModel;
    spry::Model cubeModel;
    spry::BlinnPhongRenderer::DirLight mDirLight;
    spry::Material mCurrMaterial = *spry::materials.at({ "gold" });
    spry::Scene root;
    // ImGuiView
    // std::vector<const char*> materialNames;

    void onUpdate(float deltaTime) override
    {
        float time = glfwGetTime();
        processInput(deltaTime);


        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        scene.draw();

        const auto& view = camera.getViewMatrix();
        const auto& proj = camera.getProjectionMatrix();
        const auto model = glm::mat4(1.0f);
        simpleShader.bind();
        simpleShader.setUniformMatrix("model", model);
        simpleShader.setUniformMatrix("view", view);
        simpleShader.setUniformMatrix("proj", proj);
        simpleShader.setUniformVec("viewPos", camera.mPosition);

        // Materials
        simpleShader.setUniformVec("material.ambient", mCurrMaterial.ambient);
        simpleShader.setUniformVec("material.diffuse", mCurrMaterial.diffuse);
        simpleShader.setUniformVec("material.specular", mCurrMaterial.specular);
        simpleShader.setUniformFloat("material.shininess", mCurrMaterial.shininess);
        // DirLight
        simpleShader.setUniformVec("dirLight.direction", mDirLight.direction);
        simpleShader.setUniformVec("dirLight.ambient", mDirLight.ambient);
        simpleShader.setUniformVec("dirLight.diffuse", mDirLight.diffuse);
        simpleShader.setUniformVec("dirLight.specular", mDirLight.specular);
        // spry::setCulling(true);
        // glCullFace(GL_BACK);

        root.draw(model, simpleShader);
        // objModel.draw();
        // sphere.draw();

        // cube.draw();
        // plane.draw();

        // const auto& shader = spry::ShaderManager::instance().get(spry::ShaderManager::SHAPE);
        // const auto& view = camera.getViewMatrix();
        // const auto& proj = camera.getProjectionMatrix();
        // const auto model = glm::mat4(1.0f);

        // scene.draw();
        // shader.bind();
        // shader.setUniformMatrix("view", view);
        // shader.setUniformMatrix("proj", proj);
        // texture.bind(0);
        // planeScene.draw(model, shader);

        // planeScene.getChild(0)->getChild(0)->draw(model, shader);

        // sphere.draw();
        // cube.draw();

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

    void onImguiDebugDraw(float delta) override
    {
        ImGui::Text("Update Time: %.2f", updateTime);
        ImGui::Separator();
        ImGui::SliderFloat("DirLight Direction.x", &mDirLight.direction.x, -100.0f, 100.0f);
        ImGui::SliderFloat("DirLight Direction.y", &mDirLight.direction.y, -100.0f, 100.0f);
        ImGui::SliderFloat("DirLight Direction.z", &mDirLight.direction.z, -100.0f, 100.0f);

        // renderer.debugView(delta);
    }
};

int main()
{
    TestWindow test(1000, 900);
    test.start();

    return 0;
}
