#include <array>
#include <cstdint>
#include <print>

#include "Camera.hpp"
#include "DefaultScene.hpp"
#include "Shader.hpp"
#include "glm/trigonometric.hpp"
#include "shapes/Cuboid.hpp"
#include "Texture.hpp"
#include "TextureRenderTarget.hpp"
#include "Window.hpp"
#include "FontRenderer.hpp"
#include "FontManager.hpp"

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
        , camera(width, height)
        , defaultScene(camera)
    {
        setCulling(true);
        setDepthTest(true);
        setBlending(true);
        setMouseCapture(true);
        setWireFrameMode(false);
        camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        spry::FontManager::instance().load(RES_PATH "fonts/Lato-Regular.ttf");

        defaultScene.load();

        std::array<uint8_t, 3 * 4> colors = {
            255, 0, 0, //
            0, 255, 0, //
            0, 255, 0, //
            255, 0, 0, //
        };

        imgTexture
            .create()
            .setFilterMode(GL_NEAREST)
            .setWrapMode(GL_CLAMP_TO_EDGE)
            .load(colors.data(), 2, 2, GL_RGB);

        targetTexture
            .create()
            .setFilterMode(GL_NEAREST)
            .setWrapMode(GL_CLAMP_TO_EDGE)
            .load(nullptr, 600, 600, GL_RGB);

        renderTarget.attachTexture(targetTexture);

        testShader
            .bind(RES_PATH "shaders/Textured.vert.glsl", GL_VERTEX_SHADER)
            .bind(RES_PATH "shaders/Textured.frag.glsl", GL_FRAGMENT_SHADER)
            .compile();
    }

private:
    int mWidth;
    int mHeight;
    spry::FontRenderer fontRenderer;
    spry::Camera camera;
    spry::DefaultScene defaultScene;
    spry::TextureRenderTarget renderTarget;
    spry::Texture targetTexture;
    spry::Shader testShader;
    spry::Cuboid cuboid;
    spry::Texture imgTexture;

    void onUpdate(float deltaTime) override
    {
        processInput(deltaTime);

        // First Pass
        renderTarget.bind();
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        auto orthoProj = glm::ortho(
            0.0f,
            static_cast<float>(mWidth),
            0.0f,
            static_cast<float>(mHeight));

        fontRenderer.draw(
            "Hello World",
            25.0,
            25.0,
            1.0,
            glm::vec4(glm::abs(glm::sin(getGlobalTime())), 0.8f, glm::abs(glm::cos(getGlobalTime())), 1.0f),
            orthoProj);

        renderTarget.unbind();

        // Second pass
        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        defaultScene.process(deltaTime);
        defaultScene.draw();

        auto model = glm::mat4(1.0f);
        auto view = camera.getViewMatrix();
        auto proj = camera.getProjectionMatrix();

        testShader.bind();
        testShader.setUniformMatrix("model", model);
        testShader.setUniformMatrix("view", view);
        testShader.setUniformMatrix("projection", proj);
        targetTexture.bind(0);
        cuboid.draw();

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
