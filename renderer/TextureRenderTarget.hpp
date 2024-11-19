#pragma once

#include "FrameBuffer.hpp"
#include "RenderBuffer.hpp"
#include "Texture.hpp"


namespace spry {

class TextureRenderTarget {
public:
    void bind() const;
    void unbind() const;
    void attachTexture(const Texture& texture) const;

private:
    FrameBuffer mFrameBuffer;
    RenderBuffer mRenderBuffer;
};

}