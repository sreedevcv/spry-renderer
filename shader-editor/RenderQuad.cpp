#include "RenderQuad.hpp"
#include <array>

RenderQuad::RenderQuad(RenderQuad&& fullScreenQuad)
{
    if (this != &fullScreenQuad) {
        mVAO = std::move(fullScreenQuad.mVAO);
    }
}

RenderQuad& RenderQuad::operator=(RenderQuad&& fullScreenQuad)
{
    if (this != &fullScreenQuad) {
        mVAO = std::move(fullScreenQuad.mVAO);
    }
    return *this;
}

void RenderQuad::load()
{
    std::array<float, 6> vertices { 0 };
    std::array format { 2u };

    mVAO.load(vertices, format, 6, GL_STATIC_DRAW);
}

void RenderQuad::draw() const
{
    mVAO.draw();
}
