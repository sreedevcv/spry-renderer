#pragma once

#include <cstdint>
#include <span>

#include <glad/glad.h>

namespace spry {

class VAO {
public:
    VAO();
    ~VAO() = default;

    void load(std::span<float> vertices, std::span<uint32_t> format, GLenum drawtype);
    void draw(GLenum mode = GL_TRIANGLES) const;
    void unload() const;
    void updateMesh(std::span<float> vertices) const;
    

    VAO(VAO&& mesh);
    VAO& operator=(VAO&& mesh);
    VAO(VAO& mesh) = delete;
    VAO& operator=(const VAO& mesh) = delete;

private:
    unsigned int mVertexCount;
    unsigned int mVAO;
    unsigned int mVBO;
};

}
