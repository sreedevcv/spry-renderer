#pragma once

#include <cstdint>
#include <FastNoiseLite.h>
#include <vector>
#include "Texture.hpp"

namespace spry {

class HeightMapGenerator {
public:
    void load(uint32_t width, uint32_t height, float resolution);
    const std::vector<uint8_t>& getHeightMap() const;

    static Texture createTextureFromHeightMap(
        const std::vector<uint8_t>& mBuffer,
        uint32_t width,
        uint32_t height);

    uint32_t mWidth;
    uint32_t mHeight;
    std::vector<uint8_t> mBuffer;
    FastNoiseLite mFastNoiseLite;

private:
};

}