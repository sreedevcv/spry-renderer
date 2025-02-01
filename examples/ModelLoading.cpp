#include <cstdlib>
#include <print>

#include "Camera.hpp"
#include "DefaultAxes.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Window.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : spry::Window(width, height, "Test", true)
        , mWidth { width }
        , mHeight { height }
        , camera(width, height)
    {
        spry::setCulling(true);
        spry::setDepthTest(true);
        spry::setBlending(true);
        spry::setWireFrameMode(false);
        setMouseCapture(true);
        camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        defaultScene.load(&camera);

        modelShader
            .add(RES_PATH "shaders/Model.vert", GL_VERTEX_SHADER)
            .add(RES_PATH "shaders/Model.frag", GL_FRAGMENT_SHADER)
            .compile();

        backpackModel.load(RES_PATH "models/Sponza-master/sponza.obj");
    }

private:
    int mWidth;
    int mHeight;
    spry::Camera camera;
    spry::DefaultAxes defaultScene;
    spry::Model backpackModel;
    spry::Shader modelShader;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        defaultScene.process(deltaTime);
        defaultScene.draw();

        auto model = glm::mat4(1.0f);
        // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        auto view = camera.getViewMatrix();
        auto proj = camera.getProjectionMatrix();

        modelShader.bind();
        modelShader.setUniformMatrix("model", model);
        modelShader.setUniformMatrix("view", view);
        modelShader.setUniformMatrix("proj", proj);

        backpackModel.draw();
        // closeWindow();
    }

    void processInput(float deltaTime)
    {
        if (isKeyPressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
        if (isKeyPressed(GLFW_KEY_ENTER)) {
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
};

int main()
{
    TestWindow test(800, 600);
    test.start();

    return 0;
}
