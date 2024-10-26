#include <cstdint>
#include <cstdlib>
#include <print>
#include <vector>

#include "Camera.hpp"
#include "Line.hpp"
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
    {
        glEnable(GL_CULL_FACE);
        setMouseCapture(true);
        camera.mFront = glm::vec3(0.0f, 0.0f, 0.0f);

        std::vector<float> points = {
            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //    top
            0.0f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //  front-bottom
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //  back-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, // back-right
        };

        std::vector<uint32_t> indices = {
            0, 1, 3, //
            0, 2, 1, //
            0, 3, 2, //
            1, 2, 3, //
        };

        std::vector<uint32_t> format = { 3, 3 };
        vao.loadData(std::span { points }, std::span { indices }, std::span { format });

        xAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        yAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        zAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        tetrahedronShader
            .bind(RES_PATH "shaders/Tetrahedron.vert.glsl", GL_VERTEX_SHADER)
            .bind(RES_PATH "shaders/Tetrahedron.frag.glsl", GL_FRAGMENT_SHADER)
            .compile();
        lineShader
            .bind(RES_PATH "shaders/Line.vert.glsl", GL_VERTEX_SHADER)
            .bind(RES_PATH "shaders/Line.frag.glsl", GL_FRAGMENT_SHADER)
            .compile();
    }

    ~TestWindow()
    {
        vao.deleteBuffers();
    }

private:
    int mWidth;
    int mHeight;
    spry::Shader tetrahedronShader;
    spry::Shader lineShader;
    spry::VAO vao;
    spry::Camera camera;
    spry::Line xAxis;
    spry::Line yAxis;
    spry::Line zAxis;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = camera.getProjectionMatrix();

        lineShader.use();
        lineShader.setUniformMatrix("projection", proj);
        lineShader.setUniformMatrix("view", view);
        drawAxes();

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
        if (isKeyPressed(GLFW_KEY_W)) {
            camera.processMovement(spry::Camera::movement::FORWARD, deltaTime);
        }
        if (isKeyPressed(GLFW_KEY_S)) {
            camera.processMovement(spry::Camera::movement::BACKWARD, deltaTime);
        }
        if (isKeyPressed(GLFW_KEY_A)) {
            camera.processMovement(spry::Camera::movement::LEFT, deltaTime);
        }
        if (isKeyPressed(GLFW_KEY_D)) {
            camera.processMovement(spry::Camera::movement::RIGHT, deltaTime);
        }
        if (isKeyPressed(GLFW_KEY_SPACE)) {
            camera.processMovement(spry::Camera::movement::UP, deltaTime);
        }
        if (isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            camera.processMovement(spry::Camera::movement::DOWN, deltaTime);
        }
        if (isKeyPressed(GLFW_KEY_ENTER)) {
            std::println("Position {} {} {}", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);
            std::println("LookAt {} {} {}", camera.mFront.x, camera.mFront.y, camera.mFront.z);
        }
    }

    void onMouseMove(double x_pos_in, double y_pos_in) override
    {
        float x_pos = static_cast<float>(x_pos_in);
        float y_pos = static_cast<float>(y_pos_in);

        if (camera.mouseData.firstMouse) {
            camera.mouseData.lastX = x_pos;
            camera.mouseData.lastY = y_pos;
            camera.mouseData.firstMouse = false;
        }

        float x_offset = x_pos - camera.mouseData.lastX;
        float y_offset = camera.mouseData.lastY - y_pos;

        camera.mouseData.lastX = x_pos;
        camera.mouseData.lastY = y_pos;

        camera.processMouseMovement(x_offset, y_offset, true);
    }

    void onMouseScroll(double x_offset, double y_offset) override
    {
        camera.processMouseScroll(static_cast<float>(y_offset));
    }

    void drawAxes()
    {
        auto line_model = glm::mat4(1.0f);
        lineShader.setUniformMatrix("model", line_model);
        lineShader.setUniformVec("lineColor", glm::vec3(1.0f, 0.0f, 0.0f));
        xAxis.draw();
        lineShader.setUniformVec("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));
        yAxis.draw();
        lineShader.setUniformVec("lineColor", glm::vec3(0.0f, 0.0f, 1.0f));
        zAxis.draw();
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
