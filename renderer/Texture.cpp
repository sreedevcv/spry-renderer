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
        texture.mTexture = 0;
    }
}

spry::Texture& spry::Texture::operator=(Texture&& texture)
{
    if (this != &texture) {
        mTexture = std::move(texture.mTexture);
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

void spry::Texture::load(void* data, int width, int height, GLenum dataFormat)
{
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        dataFormat,
        width,
        height,
        0,
        dataFormat,
        GL_UNSIGNED_BYTE,
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

void spry::Texture::unload() const
{
    glDeleteTextures(1, &mTexture);
    spdlog::debug("Deleted texture[{}]", mTexture);
}

void spry::Texture::load(const char* filePath)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrComponents == 1) {
            format = GL_RED;
        } else if (nrComponents == 3) {
            format = GL_RGB;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
        } else {
            spdlog::error("Unkonw number of components for pic {}", filePath);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        mWidth = width;
        mHeight = height;

        spdlog::info("Loaded texture[{}] at: {}", mTexture, filePath);
    } else {
        spdlog::debug("Failed to load Texture at path: {}", filePath);
        stbi_image_free(data);
    }
}

uint32_t spry::Texture::getID() const
{
    return mTexture;
}