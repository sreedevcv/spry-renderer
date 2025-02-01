#pragma once

#include <glad/glad.h>

#include "Shader.hpp"
#include "VAO.hpp"

namespace spry {

class FullScreenQuad {
public:
    FullScreenQuad() = default;
    ~FullScreenQuad() = default;

    FullScreenQuad(FullScreenQuad&& fullScreenQuad);
    FullScreenQuad& operator=(FullScreenQuad&& fullScreenQuad);
    FullScreenQuad(const FullScreenQuad& fullScreenQuad) = delete;
    FullScreenQuad& operator=(const FullScreenQuad& fullScreenQuad) = delete;

    void laod();
    void draw() const;

private:
    Shader mShader;
    VAO mVAO;
};

}
