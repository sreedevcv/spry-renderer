#pragma once

#include <glad/glad.h>

#include "VAO.hpp"

class RenderQuad {
public:
    RenderQuad() = default;
    ~RenderQuad() = default;

    RenderQuad(RenderQuad&& fullScreenQuad);
    RenderQuad& operator=(RenderQuad&& fullScreenQuad);
    RenderQuad(const RenderQuad& fullScreenQuad) = delete;
    RenderQuad& operator=(const RenderQuad& fullScreenQuad) = delete;

    void load();
    void draw() const;

private:
    spry::VAO mVAO;
};
