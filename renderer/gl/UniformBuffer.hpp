#pragma once

#include <cstdint>
#include <glad/glad.h>

namespace spry {

class UniformBuffer {
public:
    UniformBuffer() = default;
    ~UniformBuffer();

    UniformBuffer(UniformBuffer&& uniformBuffer);
    UniformBuffer(const UniformBuffer& uniformBuffer) = delete;
    UniformBuffer& operator=(UniformBuffer&& uniformBuffer);
    UniformBuffer& operator=(const UniformBuffer& uniformBuffer) = delete;

    void load(int size);
    void bind(uint32_t bindingPoint) const;
    void bindRange(uint32_t bindingPoint, int start, int size) const;
    void setData(uint32_t offset, uint32_t size, void* data) const;

private:
    uint32_t mUbo { 0 };
    void unload() const;
};

}