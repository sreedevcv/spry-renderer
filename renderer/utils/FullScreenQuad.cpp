#include "FullScreenQuad.hpp"
#include <array>

spry::FullScreenQuad::FullScreenQuad(FullScreenQuad&& fullScreenQuad)
{
    if (this != &fullScreenQuad) {
        mShader = std::move(fullScreenQuad.mShader);
        mVAO = std::move(fullScreenQuad.mVAO);
    }
}

spry::FullScreenQuad& spry::FullScreenQuad::operator=(FullScreenQuad&& fullScreenQuad)
{
    if (this != &fullScreenQuad) {
        mShader = std::move(fullScreenQuad.mShader);
        mVAO = std::move(fullScreenQuad.mVAO);
    }
    return *this;
}

void spry::FullScreenQuad::load()
{
    mShader
        .add(RES_PATH "shaders/ScreenSpace.vert", GL_VERTEX_SHADER)
        .add(RES_PATH "shaders/ScreenSpace.frag", GL_FRAGMENT_SHADER)
        .compile();

    std::array<float, 6> vertices { 0 };
    std::array format { 2u };

    mVAO.load(vertices, format, 6, GL_STATIC_DRAW);
}

void spry::FullScreenQuad::draw() const
{
    mShader.bind();
    mVAO.draw();
}
