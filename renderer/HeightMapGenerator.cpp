#include "HeightMapGenerator.hpp"
#include "Texture.hpp"
#include <cstdint>
#include <type_traits>

void spry::HeightMapGenerator::load(uint32_t width, uint32_t height, float resolution)
{
    mBuffer.reserve(height * width * 4);

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            uint32_t offset = (width * i + j) * 4;
            float height = (mFastNoiseLite.GetNoise(float(i) * resolution, float(j) * resolution) + 1.0f) / 2.0f;
            uint8_t heightNorm = height * 256;
            mBuffer[offset + 0] = heightNorm;
            mBuffer[offset + 1] = heightNorm;
            mBuffer[offset + 2] = heightNorm;
            mBuffer[offset + 3] = heightNorm;
        }
    }

    mWidth = width;
    mHeight = height;

    // To always delete the previously created texture if this
    // method is called multiple times
    // mHeightMap = std::move(texture);
}

const std::vector<uint8_t>& spry::HeightMapGenerator::getHeightMap() const
{
    return mBuffer;
}

spry::Texture spry::HeightMapGenerator::createTextureFromHeightMap(const std::vector<uint8_t>& mBuffer,
    uint32_t width,
    uint32_t height)
{
    
    Texture texture;
    texture.create()
        .setWrapMode(GL_REPEAT)
        .setFilterMode(GL_LINEAR)
        .load((void*)(mBuffer.data()), width, height, GL_RGBA);

    return texture;
}