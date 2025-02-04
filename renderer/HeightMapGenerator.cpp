#include "HeightMapGenerator.hpp"
#include "Texture.hpp"
#include <utility>
#include <vector>

void spry::HeightMapGenerator::load(uint32_t width, uint32_t height, float resolution)
{
    std::vector<uint8_t> buffer(height * width * 4);

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            uint32_t offset = (width * i + j) * 4;
            float height = (mFastNoiseLite.GetNoise(float(i) * resolution, float(j) * resolution) + 1.0f) / 2.0f;
            uint8_t heightNorm = height * 256;
            buffer[offset + 0] = heightNorm;
            buffer[offset + 1] = heightNorm;
            buffer[offset + 2] = heightNorm;
            buffer[offset + 3] = heightNorm;
        }
    }

    Texture texture;
    texture
        .create()
        .setWrapMode(GL_REPEAT)
        .setFilterMode(GL_LINEAR)
        .load(buffer.data(), width, height, GL_RGBA);
    
    // To always delete the previously created texture if this 
    // method is called multiple times
    mHeightMap = std::move(texture);
}

const spry::Texture& spry::HeightMapGenerator::getHeightMap() const
{
    return mHeightMap;
}