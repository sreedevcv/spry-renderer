#pragma once

#include "Cuboid.hpp"
#include "Shader.hpp"
#include <filesystem>
#include <vector>

namespace spry {

// https://github.com/emeiri/ogldev/blob/master/Common/cubemap_texture.cpp
class EquirectangularSkyMap {
public:
    EquirectangularSkyMap();
    ~EquirectangularSkyMap();

    EquirectangularSkyMap(EquirectangularSkyMap&& cubeMap);
    EquirectangularSkyMap(const EquirectangularSkyMap& cubeMap) = delete;
    EquirectangularSkyMap& operator=(EquirectangularSkyMap&& cubeMap);
    EquirectangularSkyMap& operator=(const EquirectangularSkyMap& cubeMap) = delete;

    void load(std::filesystem::path path);
    void bind(uint32_t textureUnit) const;
    void draw() const;
    const Shader& getShader() const;

private:
    uint32_t mTexID { 0 };
    Cuboid mCube;
    const Shader& mShader;
    const uint32_t numFaces = 6;

    void loadTextures(const std::vector<std::vector<float>>& faceImages, uint32_t width, uint32_t height);
    std::vector<std::vector<float>> createCubeMapImages(uint32_t width, uint32_t height, int componentCount, const float* image) const;
};

}