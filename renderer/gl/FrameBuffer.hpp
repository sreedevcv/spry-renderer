#pragma once

#include <cstdint>
namespace spry {

class FrameBuffer {
public:
    FrameBuffer() = default;
    ~FrameBuffer();

    FrameBuffer(FrameBuffer&& frameBuffer);
    FrameBuffer& operator=(FrameBuffer&& frameBuffer);
    FrameBuffer(const FrameBuffer& frameBuffer) = delete;
    FrameBuffer& operator=(const FrameBuffer& frameBuffer) = delete;

    void load();
    void bind() const;
    void unbind() const;
    void bindToDrawBuffer() const;

private:
    uint32_t mFBO { 0 };

    void unload() const;
};

}