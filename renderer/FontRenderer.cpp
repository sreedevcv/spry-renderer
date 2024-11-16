#include "FontRenderer.hpp"
#include "Shader.hpp"
#include <array>
#include <cstdint>
#include <span>

spry::FontRenderer::FontRenderer()
    : mFont(RES_PATH "fonts/Lato-Regular.ttf", 30)
{
    mShader
        .bind(RES_PATH "shaders/Font.vert", GL_VERTEX_SHADER)
        .bind(RES_PATH "shaders/Font.frag", GL_FRAGMENT_SHADER)
        .compile();

    std::array<uint32_t, 1> format = { 4 };
    std::array<float, 24> vertices {};
    mVao.load(vertices, std::span<uint32_t> { format }, GL_DYNAMIC_DRAW);
}

void spry::FontRenderer::draw(const std::string_view text, float x, float y, float scale, const glm::vec4& color, glm::mat4& ortho)
{
    mShader.use();
    mShader.setUniformMatrix("projection", ortho);
    mShader.setUniformVec("textColor", color);
    glActiveTexture(GL_TEXTURE0);

    for (const char c : text) {
        const auto& character = mFont.mCharacters[c];
        const float xPos = x + character.bearing.x * scale;
        const float yPos = y - (character.size.y - character.bearing.y) * scale;
        const float width = character.size.x * scale;
        const float height = character.size.y * scale;

        std::array<float, 24> vertices = {
            xPos, yPos + height, 0.0f, 0.0f, //
            xPos, yPos, 0.0f, 1.0f, //
            xPos + width, yPos, 1.0f, 1.0f, //

            xPos, yPos + height, 0.0f, 0.0f, //
            xPos + width, yPos, 1.0f, 1.0f, //
            xPos + width, yPos + height, 1.0f, 0.0f, //
        };

        character.texture.bind(0);
        mVao.updateMesh(std::span<float> { vertices });
        mVao.draw();
        x += (character.advance >> 6) * scale;
    }
}