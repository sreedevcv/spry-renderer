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
    {
        setCulling(false);
        setDepthTest(true);
        setBlending(true);
        setMouseCapture(true);
        setWireFrameMode(false);
    }

    ~TestWindow()
    {
    }

private:
    int mWidth;
    int mHeight;
    spry::FontRenderer fontRenderer;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 ortho = glm::ortho(
            0.0f,
            static_cast<float>(mWidth),
            0.0f,
            static_cast<float>(mHeight));

        // fontRenderer.draw(
        //     "A",
        //     25.0,
        //     25.0,
        //     1.0,
        //     glm::vec4(0.5f, 0.8f, 0.9f, 1.0f),
        //     ortho);

        fontRenderer.draw(
            "Hello World",
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
        }
    }
};

int main()
{
    TestWindow test(800, 600);
    test.start();

    return 0;
}