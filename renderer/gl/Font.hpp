#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <map>

#include "Texture.hpp"

namespace spry {

class Font {
public:
    Font(const char* filePath, uint32_t fontSize);

    Font(Font&& font);
    Font& operator=(Font&& font);
    Font& operator=(const Font& font) = delete;
    Font(const Font& font) = delete;

    struct Character {
        Texture texture; // ID handle of the glyph texture
        glm::ivec2 size; // Size of glyph
        glm::ivec2 bearing; // Offset from baseline to left/top of glyph
        long advance; // Offset to advance to next glyph
    };

    std::map<char, Character> mCharacters;
};

}