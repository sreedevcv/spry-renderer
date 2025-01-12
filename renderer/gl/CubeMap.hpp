#pragma once

#include "glm/ext/vector_float3.hpp"
#include <array>
#include <cstdint>
#include <glad/glad.h>

namespace spry {

class CubeMap {
public:
    CubeMap() = default;
    ~CubeMap();

    CubeMap(CubeMap&& cubeMap);
    CubeMap& operator=(CubeMap&& cubeMap);
    CubeMap(const CubeMap& cubeMap) = delete;
    CubeMap& operator=(const CubeMap& cubeMap) = delete;

    CubeMap& create();
    CubeMap& setWrapMode(GLenum mode = GL_LINEAR);
    CubeMap& setFilterMode(GLenum mode = GL_CLAMP_TO_EDGE);
    void load(uint32_t width);
    void bind(uint32_t texCount) const;
    uint32_t getID() const;

    struct Direction {
        GLenum mCubemapFace;
        glm::vec3 mTarget;
        glm::vec3 mUp;
    };

    static constexpr std::array<Direction, 6> directions {
        Direction { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
        Direction { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
        Direction { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
        Direction { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Direction { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
        Direction { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
    };

private:
    uint32_t mTexture { 0 };

    void unload() const;
};

}