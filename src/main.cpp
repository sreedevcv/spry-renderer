#include <cstdint>
#include <cstdlib>
#include <print>
#include <vector>

#include "Camera.hpp"
#include "DefaultScene.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "IndexedVAO.hpp"
#include "Shader.hpp"
#include "FontRenderer.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : spry::Window(width, height, "Test", true)
        , mWidth { width }
        , mHeight { height }
        , camera { mWidth, mHeight }
        , defaultScene(camera)
    {
        // glEnable(GL_CULL_FACE);
        // glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
            3, 1, 0, // first triangle
            3, 2, 1 // second triangle
        };
        std::vector<uint32_t> format = { 3, 2 };

        vao.load(
            std::span { points },
            std::span { indices },
            std::span { format },
            GL_STATIC_DRAW);

        textureShader
            .bind(RES_PATH "shaders/Textured.vert.glsl", GL_VERTEX_SHADER)
            .bind(RES_PATH "shaders/Textured.frag.glsl", GL_FRAGMENT_SHADER)
            .compile();

        defaultScene.load();

        simpleTexture
            .create()
            .setWrapMode(GL_CLAMP_TO_EDGE)
            .setFilterMode(GL_NEAREST)
            // .load(colors, 2, 2, GL_RGBA);
            .load("/home/sreedev/Pictures/Screenshot_20240505_142829.png");
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
    spry::IndexedVAO vao;
    spry::Camera camera;
    spry::DefaultScene defaultScene;
    spry::Texture simpleTexture;
    spry::Shader textureShader;
    spry::FontRenderer fontRenderer;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        defaultScene.update(deltaTime);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3((float)simpleTexture.mWidth / simpleTexture.mHeight, 1.0f, 1.0f));
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = camera.getProjectionMatrix();

        simpleTexture.bind(0);

        textureShader.use();
        textureShader.setUniformMatrix("projection", proj);
        textureShader.setUniformMatrix("view", view);
        textureShader.setUniformMatrix("model", model);
        vao.draw();

        // glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(mHeight), 0.0f, static_cast<float>(mWidth));
        glm::mat4 ortho = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
        fontRenderer.draw(
            "Hello World!!!!",
            500.0,
            100.0,
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
