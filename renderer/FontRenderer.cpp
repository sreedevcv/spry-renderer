#include "FontRenderer.hpp"
#include "Shader.hpp"
#include "VAO.hpp"

#include <array>
#include <cstdint>
#include <span>

spry::FontRenderer::FontRenderer()
    : mFont(RES_PATH "fonts/arial.ttf", 20)
{
    mShader
        .bind(RES_PATH "shaders/Font.vert", GL_VERTEX_SHADER)
        .bind(RES_PATH "shaders/Font.frag", GL_FRAGMENT_SHADER)
        .compile();

    std::array<uint32_t, 1> format = { 4 };
    std::array<float, 24> vertices {};
    mVao.load(vertices, std::span<uint32_t> { format }, 24, GL_DYNAMIC_DRAW);
}

void spry::FontRenderer::draw(const std::string_view text, float x, float y, float scale, const glm::vec4& color, const glm::mat4& ortho)
{
    mShader.use();
    mShader.setUniformMatrix("projection", ortho);
    mShader.setUniformVec("textColor", color);
    glActiveTexture(GL_TEXTURE0);

    for (const char c : text) {
        const auto& character = mFont.mCharacters[c];
        const float xPos = x + character.bearing.x * scale;
        const float yPos = y - (character.size.y - character.bearing.y) * scale;
        const float w = character.size.x * scale;
        const float h = character.size.y * scale;

        std::array<float, 24> vertices = {
            xPos, yPos + h, 0.0f, 0.0f, //
            xPos, yPos, 0.0f, 1.0f, //
            xPos + w, yPos, 1.0f, 1.0f, //

            xPos, yPos + h, 0.0f, 0.0f, //
            xPos + w, yPos, 1.0f, 1.0f, //
            xPos + w, yPos + h, 1.0f, 0.0f, //
        };

        character.texture.bind(0);
        mVao.updateMesh(vertices);
        mVao.draw(GL_TRIANGLES);
        x += (character.advance >> 6) * scale;
    }

    return;
}