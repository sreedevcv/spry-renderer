#include "Texture.hpp"

#include <print>

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

spry::Texture::Texture(Texture&& texture)
{
    if (this != &texture) {
        mTexture = std::move(texture.mTexture);
        mWidth = std::move(texture.mWidth);
        mHeight = std::move(texture.mHeight);
        texture.mTexture = 0;
    }
}

spry::Texture& spry::Texture::operator=(Texture&& texture)
{
    if (this != &texture) {
        unload();
        mTexture = std::move(texture.mTexture);
        mWidth = std::move(texture.mWidth);
        mHeight = std::move(texture.mHeight);
        texture.mTexture = 0;
    }
    return *this;
}

spry::Texture& spry::Texture::create()
{
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    return *this;
}

spry::Texture& spry::Texture::setWrapMode(GLenum mode)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    return *this;
}

spry::Texture& spry::Texture::setFilterMode(GLenum mode)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
    return *this;
}

spry::Texture& spry::Texture::setBorderColor(std::span<float, 4> borderColor)
{
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());
    return *this;
}

spry::Texture& spry::Texture::setCompareModeAndFunc(GLenum compareMode, GLenum compareFunc)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, compareMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, compareFunc);
    return *this;
}

// Load a buffer of bytes (uint8_t) into gpu memory
void spry::Texture::load(void* data, int width, int height, GLenum dataFormat, GLenum type)
{
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        dataFormat,
        width,
        height,
        0,
        dataFormat,
        type,
        data);
    glGenerateMipmap(GL_TEXTURE_2D);

    mWidth = width;
    mHeight = height;

    spdlog::debug("Loaded texture[{}]", mTexture);
}

void spry::Texture::bind(int tex_count) const
{
    glActiveTexture(GL_TEXTURE0 + tex_count);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

void spry::Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void spry::Texture::unload() const
{
    glDeleteTextures(1, &mTexture);
    spdlog::debug("Deleted texture[{}]", mTexture);
}

// Load an image from memory as a texture
void spry::Texture::load(const char* filePath)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrComponents == 1) {
            format = GL_RED;
        } else if (nrComponents == 2) {
            format = GL_RG;
        } else if (nrComponents == 3) {
            format = GL_RGB;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
        } else {
            spdlog::error("Unkonw number of components({}) for pic {}", nrComponents, filePath);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        mWidth = width;
        mHeight = height;

        spdlog::info("Loaded texture[{}] at: {}[{}x{}x{}]", mTexture, filePath, mWidth, mHeight, nrComponents);
    } else {
        spdlog::error("Failed to load Texture at path: {}", filePath);
        stbi_image_free(data);
    }
}

uint32_t spry::Texture::getID() const
{
    return mTexture;
}
