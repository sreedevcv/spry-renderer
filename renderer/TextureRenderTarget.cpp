#include "TextureRenderTarget.hpp"

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

void spry::TextureRenderTarget::attachTexture(const Texture& texture) const
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
    mFrameBuffer.unbind();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Framebuffer for TextureRenderTarget is not complete!");
    } else {
        spdlog::info("Attached TextureRenderTarget to texture[{}]", texture.getID());
    }
}