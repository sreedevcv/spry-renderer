#include "TextureViewer.hpp"
#include "IndexedVAO.hpp"
#include "ShaderManager.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <array>
#include <cstdint>

spry::DebugTextureViewer::DebugTextureViewer()
    : mShader(ShaderManager::instance().loadAndGet(ShaderManager::SHAPE))
{
}

void spry::DebugTextureViewer::load(uint32_t x, uint32_t y, uint32_t screenWidth, uint32_t screenHeight)
{
    // model = glm::translate(glm::mat4(1.0f), glm::vec3(float(x), float(y), 0.0f));
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    proj = glm::ortho(
        0.0f,
        static_cast<float>(screenWidth),
        0.0f,
        static_cast<float>(screenHeight));

    const float xf = float(x);
    const float yf = float(y);
    // const float wf = float(screenWidth);
    // const float hf = float(screenHeight);
    const float wf = float(20);
    const float hf = float(20);

    std::array<float, 4 * (3 + 3 + 2)> vertices = {
        xf, yf + hf, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //
        xf, yf, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
        xf + wf, yf, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, //
        xf + wf, yf + hf, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //
    };
    std::array<uint32_t, 6> indices = {
        0, 1, 2, //
        0, 2, 3, //
    };
    std::array<uint32_t, 3> format = { 3, 3, 2 };

    mVao.load(vertices, indices, format, GL_STATIC_DRAW);
}

void spry::DebugTextureViewer::draw(const Texture& texture) const
{
    mShader.bind();
    mShader.setUniformMatrix("model", model);
    mShader.setUniformMatrix("view", view);
    mShader.setUniformMatrix("proj", proj);
    texture.bind(0);
    mVao.draw();
}
