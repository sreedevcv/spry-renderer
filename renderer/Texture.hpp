#pragma once

#include <cstdint>

namespace spry {

class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    void bind(int texCount);
    void load(const char* filePath);
    void load(void* data, int width, int height);
    void unload();

private:
    uint32_t mTexture;
};

}