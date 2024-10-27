#include <cstdint>
#include <cstdlib>
#include <print>
#include <vector>

#include "Camera.hpp"
#include "DefaultScene.hpp"
#include "Window.hpp"
#include "VAO.hpp"
#include "Shader.hpp"

#include "FastNoiseLite.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : mHeight { height }
        , mWidth { width }
        , spry::Window(width, height, "Test", true)
        , camera { mWidth, mHeight }
        , defaultScene(camera)
    {
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        setWireFrameMode(true);
        setMouseCapture(true);
        camera.mFront = glm::vec3(0.0f, 0.0f, 0.0f);

        // std::vector<float> points = {
        //     0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //    top
        //     0.0f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //  front-bottom
        //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //  back-left
        //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, // back-right
        // };

        // std::vector<uint32_t> indices = {
        //     0, 1, 3, //
        //     0, 2, 1, //
        //     0, 3, 2, //
        //     1, 2, 3, //
        // };

        // std::vector<uint32_t> format = { 3, 3 };

        std::vector<float> points = {
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, //
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, //
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, //
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, //
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, //
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, //
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //

        };

        std::vector<uint32_t> indices = {
            1, 0, 2, //
            1, 2, 3, //
            0, 4, 2, //
            2, 4, 6, //
            4, 5, 6, //
            5, 6, 7, //
            5, 1, 3, //
            5, 3, 7, //
            0, 1, 5, //
            0, 5, 4, //
            6, 7 ,3, //
            6, 3, 2, //
        };
        std::vector<uint32_t> format = {3, 3};

        vao.load(std::span { points }, std::span { indices }, std::span { format });

        tetrahedronShader
            .bind(RES_PATH "shaders/Tetrahedron.vert.glsl", GL_VERTEX_SHADER)
            .bind(RES_PATH "shaders/Tetrahedron.frag.glsl", GL_FRAGMENT_SHADER)
            .compile();

        defaultScene.load();
    }

    ~TestWindow()
    {
        vao.unload();
        tetrahedronShader.unload();
        defaultScene.unload();
    }

private:
    int mWidth;
    int mHeight;
    spry::Shader tetrahedronShader;
    spry::VAO vao;
    spry::Camera camera;
    spry::DefaultScene defaultScene;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        defaultScene.update(deltaTime);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = camera.getProjectionMatrix();

        tetrahedronShader.use();
        tetrahedronShader.setUniformMatrix("projection", proj);
        tetrahedronShader.setUniformMatrix("view", view);
        tetrahedronShader.setUniformMatrix("model", model);
        vao.draw();

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
    std::println("Hello World!");

    TestWindow test(800, 600);
    test.start();

    // FastNoiseLite noise;
    // noise.SetNoiseType(FastNoiseLite::NoiseType_Value);
    // float prev = 0;

    // for (int i = 0; i < 20; i++) {
    //     float current = noise.GetNoise(1.0f, (float)i * 10);
    //     std::println("{} {}", 10 + current * 20, current - prev);
    //     prev = current;
    // }

    return 0;
}
