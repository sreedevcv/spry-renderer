#include "UniformBuffer.hpp"
#include <GL/glext.h>
#include <utility>

spry::UniformBuffer::~UniformBuffer()
{
    unload();
}

spry::UniformBuffer::UniformBuffer(UniformBuffer&& uniformBuffer)
{
    if (this != &uniformBuffer) {
        mUbo = uniformBuffer.mUbo;
        uniformBuffer.mUbo = 0;
    }
}

spry::UniformBuffer& spry::UniformBuffer::operator=(UniformBuffer&& uniformBuffer)
{
    if (this != &uniformBuffer) {
        unload();

        mUbo = std::move(uniformBuffer.mUbo);
        uniformBuffer.mUbo = 0;
    }

    return *this;
}

void spry::UniformBuffer::load(int size)
{
    glGenBuffers(1, &mUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, mUbo);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void spry::UniformBuffer::bind(uint32_t bindingPoint) const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, mUbo);
}

void spry::UniformBuffer::bindRange(uint32_t bindingPoint, int start, int size) const
{
    glBindBufferRange(GL_UNIFORM_BLOCK_BINDING, bindingPoint, mUbo, start, size);
}

// Offset and sizes should match alignment
void spry::UniformBuffer::setData(uint32_t offset, uint32_t size, void* data) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, mUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void spry::UniformBuffer::unload() const
{
    glDeleteBuffers(1, &mUbo);
}