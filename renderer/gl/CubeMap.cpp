#include "CubeMap.hpp"

#include <cstdint>
#include <utility>

spry::CubeMap::~CubeMap()
{
    unload();
}

void spry::CubeMap::unload() const
{
    glDeleteTextures(1, &mTexture);
}

spry::CubeMap::CubeMap(CubeMap&& cubeMap)
{
    if (this != &cubeMap) {
        mTexture = std::move(cubeMap.mTexture);
        cubeMap.mTexture = 0;
    }
}

spry::CubeMap& spry::CubeMap::operator=(CubeMap&& cubeMap)
{
    if (this != &cubeMap) {
        unload();

        mTexture = std::move(cubeMap.mTexture);
        cubeMap.mTexture = 0;
    }

    return *this;
}

spry::CubeMap& spry::CubeMap::create()
{
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    return *this;
}

spry::CubeMap& spry::CubeMap::setWrapMode(GLenum mode)
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mode);
    return *this;
}

spry::CubeMap& spry::CubeMap::setFilterMode(GLenum mode)
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mode);
    return *this;
}

void spry::CubeMap::load(void* data, int width, int height, GLenum dataFormat, GLenum type)
{
    for (uint32_t i = 0; i < faceCount; i++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            dataFormat,
            width,
            height,
            0,
            dataFormat,
            type,
            data);
    }
}

uint32_t spry::CubeMap::getID() const
{
    return mTexture;
}

void spry::CubeMap::bind(uint32_t texCount) const
{
    glActiveTexture(GL_TEXTURE0 + texCount);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
}