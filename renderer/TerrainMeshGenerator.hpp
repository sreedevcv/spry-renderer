#pragma once

#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include <cstdint>

namespace spry {

class TerrainMeshGenerator {
public:
    TerrainMeshGenerator();

    void load(int32_t width, int32_t height);
    void draw(const Texture& heightMap, const Camera& camera) const;

private:
    VAO mVAO;
    Shader mTesselationShader;
};

}