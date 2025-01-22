#include "TextureRenderTarget.hpp"

#include "CubeMap.hpp"
#include "FrameBuffer.hpp"
#include "spdlog/spdlog.h"
#include <glad/glad.h>

void spry::TextureRenderTarget::load()
{
    mFrameBuffer.load();
    mRenderBuffer.load();
}

void spry::TextureRenderTarget::bind() const
{
    mFrameBuffer.bind();
}

void spry::TextureRenderTarget::unbind() const
{
    mFrameBuffer.unbind();
}

// Attach a texture as the Framebuffer to be rendered on
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
        spdlog::error("Framebuffer for TextureRenderTarget(Depth) with texture is not complete!");
    } else {
        spdlog::info("Attached TextureRenderTarget(Depth) to texture[{}]", texture.getID());
    }

    mFrameBuffer.unbind();
}

void spry::TextureRenderTarget::bindTextureColor(GLenum attachment, const CubeMap& texture) const
{
    mFrameBuffer.bindToDrawBuffer();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, attachment, texture.getID(), 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void spry::TextureRenderTarget::attachTextureDepth(const CubeMap& cubemap) const
{
    mFrameBuffer.bind();
    glFramebufferTexture(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        cubemap.getID(),
        0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Framebuffer for TextureRenderTarget(Depth) with Cubemap is not complete!");
    } else {
        spdlog::info("Attached TextureRenderTarget(Depth) to Cubemap[{}]", cubemap.getID());
    }

    mFrameBuffer.unbind();
}