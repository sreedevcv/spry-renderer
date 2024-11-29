#pragma once

#include "IndexedVAO.hpp"
#include "Texture.hpp"

#include <vector>

namespace spry {

class Plane {
public:
    void load(uint32_t width, uint32_t height);
    void draw(const Texture& texture) const;

    struct Buffer {
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        std::vector<uint32_t> format;
    };

    Buffer getBuffers(uint32_t width, uint32_t height) const;

private:
    IndexedVAO mVao;
};

}