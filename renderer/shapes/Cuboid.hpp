#pragma once

#include "IndexedVAO.hpp"

namespace spry {

class Cuboid {
public:
    Cuboid();

    void draw() const;

private:
    IndexedVAO mVAO;
};

}