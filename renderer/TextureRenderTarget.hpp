#pragma once

#include "FrameBuffer.hpp"
#include "RenderBuffer.hpp"
#include "Texture.hpp"


namespace spry {

class TextureRenderTarget {
public:
    void load();
    void bind() const;
    void unbind() const;
    void attachTextureColor(const Texture& texture) const;
    void attachTextureDepth(const Texture& texture) const;

private:
    FrameBuffer mFrameBuffer;
    RenderBuffer mRenderBuffer;
};

}