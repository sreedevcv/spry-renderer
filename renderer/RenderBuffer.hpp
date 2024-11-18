#pragma once

#include <cstdint>

namespace spry {

class RenderBuffer {
public:
    RenderBuffer();
    ~RenderBuffer();

    RenderBuffer(RenderBuffer&& renderBuffer);
    RenderBuffer& operator=(RenderBuffer&& renderBuffer);
    RenderBuffer& operator=(const RenderBuffer& renderBuffer) = delete;
    RenderBuffer(const RenderBuffer& renderBuffer) = delete;

    void unload() const;
    void bind() const;
    void unbind() const;
    void attachToDepthAndStencil(uint32_t width, uint32_t height) const;

private:
    uint32_t mRBO;
};

}