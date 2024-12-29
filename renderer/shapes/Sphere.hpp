#pragma once

#include "IndexedVAO.hpp"

namespace spry {

class Sphere {
public:
    void load(uint32_t width, uint32_t height);
    void draw() const;

private:
    IndexedVAO mVao;
};


}