#include "RenderBuffer.hpp"

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <utility>

void spry::RenderBuffer::load()
{
    glGenRenderbuffers(1, &mRBO);
    spdlog::debug("Created renderbuffer[{}]", mRBO);
}

spry::RenderBuffer::~RenderBuffer()
{
    unload();
}

void spry::RenderBuffer::unload() const
{
    glDeleteRenderbuffers(1, &mRBO);
    spdlog::debug("Deleted renderbuffer[{}]", mRBO);
}

spry::RenderBuffer::RenderBuffer(RenderBuffer&& renderBuffer)
{
    if (this != &renderBuffer) {
        mRBO = std::move(renderBuffer.mRBO);
        mRBO = 0;
    }
}

spry::RenderBuffer& spry::RenderBuffer::operator=(RenderBuffer&& renderBuffer)
{
    if (this != &renderBuffer) {
        mRBO = std::move(renderBuffer.mRBO);
        mRBO = 0;
    }
    return *this;
}

void spry::RenderBuffer::bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
}

void spry::RenderBuffer::unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void spry::RenderBuffer::attachToDepthAndStencil(uint32_t width, uint32_t height) const
{
    // Configure rbo
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        width,
        height);

    // Attach rbo to currently bound framebuffer
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        mRBO);
}