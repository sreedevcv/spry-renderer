#include <cstdint>
#include <print>
#include <vector>

#include "Window.hpp"
#include "VAO.hpp"
#include "Shader.hpp"

#include "FastNoiseLite.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

class TestWindow : public spry::Window {
public:
    TestWindow()
        : spry::Window(600, 400, "Test", true)
    {
        std::vector<float> points = {
            0.0f, 1.0f, 0.0f, //    top
            0.0f, -0.5f, -0.5f, //  front-bottom
            -0.5f, -0.5f, 0.5f, //  back-left
            0.5f, -0.5f, 0.5f, // back-right
        };

        std::vector<uint32_t> indices = {
            0, 1, 3, //
            0, 2, 1, //
            0, 3, 2, //
            1, 2, 3, //
        };

        std::vector<uint32_t> format = { 3 };
        vao.loadData(std::span { points }, std::span { indices }, std::span { format });

        shader
            .bind(RES_PATH "shaders/Tetrahedron.vert.glsl", GL_VERTEX_SHADER)
            .bind(RES_PATH "shaders/Tetrahedron.frag.glsl", GL_FRAGMENT_SHADER)
            .compile();
    }

    ~TestWindow()
    {
        vao.deleteBuffers();
    }

private:
    spry::Shader shader;
    spry::VAO vao;

    void onUpdate(float deltaTime) override
    {
        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)getGlobalTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(100.0f), 600.0f / 400.0f, 0.5f, 100.0f);

        shader.use();
        shader.set_uniform_matrix("projection", proj);
        shader.set_uniform_matrix("view", view);
        shader.set_uniform_matrix("model", model);

        vao.draw();
    }
};

int main()
{
    std::println("Hello World!");

    TestWindow test;
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
