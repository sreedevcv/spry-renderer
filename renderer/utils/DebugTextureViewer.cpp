#include "DebugTextureViewer.hpp"

#include "IndexedVAO.hpp"
#include "ShaderManager.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include <array>

spry::DebugTextureViewer::DebugTextureViewer()
    : mShader(ShaderManager::instance().loadAndGet(ShaderManager::SHAPE))
{
}

void spry::DebugTextureViewer::load(glm::ivec4 dimensions, uint32_t screenWidth, uint32_t screenHeight)
{
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    proj = glm::ortho(
        0.0f,
        static_cast<float>(screenWidth),
        0.0f,
        static_cast<float>(screenHeight));

    const float xf = float(dimensions.x);
    const float yf = float(dimensions.y);
    const float wf = float(dimensions.z);
    const float hf = float(dimensions.w);

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

    mVao.load(vertices, indices, format, GL_DYNAMIC_DRAW);
}

void spry::DebugTextureViewer::update(glm::ivec4 dimensions, uint32_t screenWidth, uint32_t screenHeight)
{
    proj = glm::ortho(
        0.0f,
        static_cast<float>(screenWidth),
        0.0f,
        static_cast<float>(screenHeight));

    const float xf = float(dimensions.x);
    const float yf = float(dimensions.y);
    const float wf = float(dimensions.z);
    const float hf = float(dimensions.w);

    std::array<float, 4 * (3 + 3 + 2)> vertices = {
        xf, yf + hf, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //
        xf, yf, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
        xf + wf, yf, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, //
        xf + wf, yf + hf, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //
    };

    mVao.updateMesh(vertices);
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
