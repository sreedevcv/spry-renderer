#pragma once

#include <cstdint>
#include <span>

#include <glad/glad.h>

namespace spry {

class VAO {
public:
    VAO() = default;
    ~VAO() = default;

    void load(std::span<float> vertices, std::span<uint32_t> indices, std::span<uint32_t> format);
    void load(std::span<float> vertices, std::span<uint32_t> format);
    void draw(GLenum mode = GL_TRIANGLES) const;
    void unload() const;

    VAO(VAO& mesh);
    VAO(VAO&& mesh);
    VAO& operator=(VAO& mesh);
    VAO& operator=(VAO&& mesh);

private:
    int mVertexCount;

    bool mDataLoaded = false;
    bool mWithIndexArray = false;

    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;
};

}
