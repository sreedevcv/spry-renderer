#include "FrameBuffer.hpp"

#include <glad/glad.h>
#include <utility>
#include <spdlog/spdlog.h>

spry::FrameBuffer::~FrameBuffer()
{
    unload();
}

void spry::FrameBuffer::unload() const
{
    glDeleteFramebuffers(1, &mFBO);
    spdlog::debug("Deleted framebuffer[{}]", mFBO);
}

void spry::FrameBuffer::load()
{
    glGenFramebuffers(1, &mFBO);
    spdlog::debug("Created framebuffer[{}]", mFBO);
}

spry::FrameBuffer::FrameBuffer(FrameBuffer&& frameBuffer)
{
    if (this != &frameBuffer) {
        mFBO = std::move(frameBuffer.mFBO);
        mFBO = 0;
    }
}

spry::FrameBuffer& spry::FrameBuffer::operator=(FrameBuffer&& frameBuffer)
{
    if (this != &frameBuffer) {
        unload();

        mFBO = std::move(frameBuffer.mFBO);
        mFBO = 0;
    }

    return *this;
}

void spry::FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void spry::FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}