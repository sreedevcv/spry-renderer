#include <array>
#include <cstdint>
#include <print>

#include "Camera.hpp"
#include "DefaultScene.hpp"
#include "Shader.hpp"
#include "VAO.hpp"
#include "Window.hpp"
#include "FontRenderer.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : spry::Window(width, height, "Test", true)
        , mWidth { width }
        , mHeight { height }
        , camera { mWidth, mHeight }
        // , defaultScene(camera)
    {
        setCulling(false);
        setDepthTest(true);
        setBlending(true);
        setMouseCapture(true);
        setWireFrameMode(false);

        camera.mFront = glm::vec3(0.0f, 0.0f, 0.0f);

        // defaultScene.load();

        // std::array<float, 6 * 6> vertices = {
        //     0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        //     1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        //     0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //

        //     1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        //     1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        //     0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        // };
        // std::array<uint32_t, 2> format = {
        //     3,
        //     3,
        // };

        // vao.load(vertices, format, 6, GL_STATIC_DRAW);

        // quadShader
        //     .bind(RES_PATH "shaders/Tetrahedron.vert", GL_VERTEX_SHADER)
        //     .bind(RES_PATH "shaders/Tetrahedron.frag", GL_FRAGMENT_SHADER)
        //     .compile();
    }

    ~TestWindow()
    {
        // defaultScene.unload();
    }

private:
    int mWidth;
    int mHeight;
    spry::Camera camera;
    // spry::DefaultScene defaultScene;
    spry::FontRenderer fontRenderer;
    // spry::VAO vao;
    // spry::Shader quadShader;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // defaultScene.update(deltaTime);

        // auto model = glm::mat4(1.0f);
        // auto view = camera.getViewMatrix();
        // auto proj = camera.getProjectionMatrix();
        // quadShader.use();
        // quadShader.setUniformMatrix("model", model);
        // quadShader.setUniformMatrix("view", view);
        // quadShader.setUniformMatrix("projection", proj);
        // vao.draw();

        glm::mat4 ortho = glm::ortho(
            0.0f,
            static_cast<float>(mWidth),
            0.0f,
            static_cast<float>(mHeight));

        fontRenderer.draw(
            "A",
            25.0,
            25.0,
            1.0,
            glm::vec4(0.5f, 0.8f, 0.9f, 1.0f),
            ortho);

        fontRenderer.draw(
            "B",
            250.0,
            250.0,
            1.0,
            glm::vec4(0.5f, 0.8f, 0.9f, 1.0f),
            ortho);

        // closeWindow();
    }

    void processInput(float deltaTime)
    {
        if (isKeyPressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
        if (isKeyPressed(GLFW_KEY_ENTER)) {
            std::println("Position {} {} {}", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);
            std::println("LookAt {} {} {}", camera.mFront.x, camera.mFront.y, camera.mFront.z);
        }

        camera.defaultInputProcess(*this, deltaTime);
    }

    void onMouseMove(double xPosIn, double yPosIn) override
    {
        camera.defaultMousePan(xPosIn, yPosIn);
    }

    void onMouseScroll(double xOffset, double yOffset) override
    {
        camera.processMouseScroll(static_cast<float>(yOffset));
    }
};

int main()
{
    TestWindow test(800, 600);
    test.start();

    return 0;
}
