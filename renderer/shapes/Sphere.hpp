#pragma once

#include "Drawable.hpp"
#include "IndexedVAO.hpp"

namespace spry {

class Sphere : public Drawable {
public:
    void load(uint32_t width, uint32_t height);
    virtual void draw() const override;

private:
    IndexedVAO mVao;
};

}