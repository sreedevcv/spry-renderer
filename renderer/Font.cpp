#include "Font.hpp"
#include "Texture.hpp"
#include "glm/ext/vector_int2.hpp"

#include <print>

#include <glad/glad.h>
#include <freetype/freetype.h>
#include <utility>
#include <spdlog/spdlog.h>

spry::Font::Font(const char* filePath, uint32_t fontSize)
{
    FT_Library ft;

    if (FT_Init_FreeType(&ft)) {
        spdlog::error("Could not init freetype");
        return;
    }

    FT_Face face;

    if (FT_New_Face(ft, filePath, 0, &face)) {
        spdlog::error("Could not load {}", filePath);
        return;
    }

    /* Pixel width of zero of means width is dynamically calculated based on
        height
    */
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            spdlog::error("Could not load {} char", c);
            continue;
        }

        Texture texture;
        texture
            .create()
            .setWrapMode(GL_CLAMP_TO_EDGE)
            .setFilterMode(GL_LINEAR)
            .load(
                face->glyph->bitmap.buffer,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                GL_RED);

        Character character = {
            .texture = std::move(texture),
            .size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            .bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            .advance = face->glyph->advance.x
        };

        mCharacters.insert(std::pair<char, Character>(c, std::move(character)));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    spdlog::info("Loaded font: {}", filePath);
}

spry::Font::Font(Font&& font)
{
    if (this != &font) {
        mCharacters = std::move(font.mCharacters);
    }
}

spry::Font& spry::Font::operator=(Font&& font)
{
    if (this != &font) {
        mCharacters = std::move(font.mCharacters);
    }
    return *this;
}
