#pragma once

#include "Drawable.hpp"
#include "IndexedVAO.hpp"

namespace spry {

class Cuboid : public Drawable {
public:
    Cuboid();

    virtual void draw() const override;

private:
    IndexedVAO mVAO;
};

}