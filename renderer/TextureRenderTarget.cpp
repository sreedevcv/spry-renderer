#include "TextureRenderTarget.hpp"

#include "FrameBuffer.hpp"
#include "spdlog/spdlog.h"
#include <glad/glad.h>

void spry::TextureRenderTarget::bind() const
{
    mFrameBuffer.bind();
}

void spry::TextureRenderTarget::unbind() const
{
    mFrameBuffer.unbind();
}

// Attach a texture as the Framebuffer to ben rendered on
void spry::TextureRenderTarget::attachTextureColor(const Texture& texture) const
{
    mFrameBuffer.bind();

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        texture.getID(),
        0);

    mRenderBuffer.bind();
    mRenderBuffer.attachToDepthAndStencil(texture.mWidth, texture.mHeight);
    mRenderBuffer.unbind();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Framebuffer for TextureRenderTarget is not complete!");
    } else {
        spdlog::info("Attached TextureRenderTarget(Color) to texture[{}]", texture.getID());
    }

    mFrameBuffer.unbind();
}

void spry::TextureRenderTarget::attachTextureDepth(const Texture& texture) const
{
    mFrameBuffer.bind();
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        texture.getID(),
        0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Framebuffer for TextureRenderTarget(Depth) is not complete!");
    } else {
        spdlog::info("Attached TextureRenderTarget(Depth) to texture[{}]", texture.getID());
    }

    mFrameBuffer.unbind();
}