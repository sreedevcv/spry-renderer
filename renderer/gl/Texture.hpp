#pragma once

#include <cstdint>

#include <glad/glad.h>
#include <span>

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
    Texture& setBorderColor(std::span<float, 4> borderColor);
    Texture& setCompareModeAndFunc(GLenum compareMode, GLenum compareFunc);

    void bind(int texCount) const;
    void unbind() const;
    void load(const char* filePath);
    void load(void* data, int width, int height, GLenum dataFormat, GLenum type = GL_UNSIGNED_BYTE);
    uint32_t getID() const;

private:
    uint32_t mTexture { 0 };

    void unload() const;
};

}