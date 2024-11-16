#pragma once

#include "VAO.hpp"
namespace spry {

class IndexedVAO {
public:
    IndexedVAO();

    void load(std::span<float> vertices, std::span<uint32_t> indices, std::span<uint32_t> format, GLenum drawtype);
    void draw(GLenum mode = GL_TRIANGLES) const;
    void unload() const;
    void updateMesh(std::span<float> vertices) const;

    IndexedVAO(IndexedVAO&& mesh);
    IndexedVAO& operator=(IndexedVAO&& mesh);
    IndexedVAO(IndexedVAO& mesh) = delete;
    IndexedVAO& operator=(const IndexedVAO& mesh) = delete;

private:
    unsigned int mVertexCount;
    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;
};

}