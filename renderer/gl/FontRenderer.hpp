#pragma once

#include <string_view>

#include "Shader.hpp"
#include "VAO.hpp"


namespace spry {

class FontRenderer {
public:
    FontRenderer();
    // FIXEME:: REmove ortho mat4 by creating a global context singleton for rendrer
    void draw(const std::string_view text, float x, float y, float scale, const glm::vec4& color, const glm::mat4& ortho);

private:
    const Shader& mShader;
    VAO mVao;
};

}
