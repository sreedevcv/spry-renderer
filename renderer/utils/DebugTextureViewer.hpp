#pragma once

#include "IndexedVAO.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_int4.hpp"
#include <cstdint>

namespace spry {

class DebugTextureViewer {
public:
    DebugTextureViewer();
    void load(glm::ivec4 dimensions, uint32_t screenWidth, uint32_t screenHeight);
    void update(glm::ivec4 dimensions, uint32_t screenWidth, uint32_t screenHeight);
    void draw(const Texture& texture) const;

private:
    IndexedVAO mVao;
    const Shader& mShader;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

}