#pragma once

#include "Texture.hpp"
#include <cstdint>
#include <FastNoiseLite.h>

namespace spry {

class HeightMapGenerator {
public:

    void load(uint32_t width, uint32_t height, float resolution);
    const Texture& getHeightMap() const;

private:
    FastNoiseLite mFastNoiseLite;
    Texture mHeightMap;
};

}