#pragma once

#include <cstdint>

#include <glad/glad.h>

namespace spry {

class Texture {
public:
    uint32_t mWidth;
    uint32_t mHeight;

    Texture() = default;
    ~Texture() = default;

    Texture(Texture&& texture);
    Texture& operator=(Texture&& texture);
    Texture(const Texture& texture) = delete;
    Texture& operator=(const Texture& texture) = delete;

    Texture& create();
    Texture& setWrapMode(GLenum mode);
    Texture& setFilterMode(GLenum mode);

    void bind(int texCount) const;
    void load(const char* filePath);
    void load(void* data, int width, int height, GLenum dataFormat);
    void unload() const;
    uint32_t getID() const;

private:
    uint32_t mTexture;
};

}