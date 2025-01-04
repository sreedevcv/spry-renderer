#pragma once

#include <glad/glad.h>
#include <utility>

#include "TextureRenderTarget.hpp"

namespace spry {

template <GLenum... Buffers>
class RenderPass {
public:
    template <typename Func>
    void draw(const Func func)
    {
        glClear((Buffers | ...));
        mRenderTarget.bind();

        func();

        mRenderTarget.unbind();
    }

    inline void setRenderTarget(TextureRenderTarget&& renderTarget)
    {
        mRenderTarget = std::move(renderTarget);
    }

private:
    TextureRenderTarget mRenderTarget;
};

}