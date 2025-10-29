#pragma once

#include <array>
#include <cstdint>
#include <FastNoiseLite.h>
#include <utility>
#include <vector>
#include "Texture.hpp"

namespace spry {

class HeightMapGenerator {
public:
    HeightMapGenerator();

    void load(uint32_t width, uint32_t height);
    void generate(uint32_t width, uint32_t height);

    const std::vector<uint8_t>& getHeightMap() const;

    Texture createTextureFromHeightMap(uint32_t width, uint32_t height);

    uint32_t mWidth;
    uint32_t mHeight;
    FastNoiseLite mFastNoiseLite;
    float mPower = 1.0f;
    static constexpr int mCount = 3;
    std::array<std::pair<FastNoiseLite, float>, mCount> mGenerators;

private:
    std::vector<uint8_t> mBuffer;
};

}
