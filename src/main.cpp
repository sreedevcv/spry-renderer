#include <cstdint>
#include <cstdlib>
#include <print>
#include <vector>

#include "Camera.hpp"
#include "DefaultScene.hpp"
#include "Texture.hpp"
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
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        // setWireFrameMode(true);
        setMouseCapture(true);
        camera.mFront = glm::vec3(0.0f, 0.0f, 0.0f);


        std::vector<float> points = {
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f // top left
        };

        std::vector<uint32_t> indices = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
        };
        std::vector<uint32_t> format = { 3, 2 };

        vao.load(std::span { points }, std::span { indices }, std::span { format });

        textureShader
            .bind(RES_PATH "shaders/Textured.vert.glsl", GL_VERTEX_SHADER)
            .bind(RES_PATH "shaders/Textured.frag.glsl", GL_FRAGMENT_SHADER)
            .compile();

        defaultScene.load();

        uint8_t colors[] = {
            255, 0, 0, 255, //
            0, 255, 0, 255, //
            0, 255, 0, 255, //
            255, 0, 0, 255, //
        };

        simpleTexture.load(colors, 2, 2);
    }

    ~TestWindow()
    {
        vao.unload();
        textureShader.unload();
        simpleTexture.unload();
        defaultScene.unload();
    }

private:
    int mWidth;
    int mHeight;
    spry::VAO vao;
    spry::Camera camera;
    spry::DefaultScene defaultScene;
    spry::Texture simpleTexture;
    spry::Shader textureShader;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        defaultScene.update(deltaTime);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = camera.getProjectionMatrix();

        simpleTexture.bind(0);

        textureShader.use();
        textureShader.setUniformMatrix("projection", proj);
        textureShader.setUniformMatrix("view", view);
        textureShader.setUniformMatrix("model", model);
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
