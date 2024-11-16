#pragma once

#include <string_view>

#include "Shader.hpp"
#include "VAO.hpp"
#include "Font.hpp"


namespace spry {

class FontRenderer {
public:
    FontRenderer();
    // FIXEME:: REmove ortho mat4 by creating a global context singleton for rendrer
    void draw(const std::string_view text, float x, float y, float scale, glm::vec4 color, glm::mat4& ortho);

private:
    Shader mShader;
    VAO mVao;
    Font mFont;
};

}