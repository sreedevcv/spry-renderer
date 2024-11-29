#pragma once

#include "glad/glad.h"
#include <cassert>
#include <span>

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

    template <typename T>
    void load(std::span<T> vertices, std::span<uint32_t> indices, std::span<uint32_t> sizes, std::span<uint32_t> offset, GLenum drawtype)
    {
        mIndexedDrawCount = indices.size();

        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glGenBuffers(1, &mEBO);
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), (void*)vertices.data(), drawtype);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), (void*)indices.data(), drawtype);

        for (auto i = 0ul; i < sizes.size(); i++) {
            int attribSize = sizes[i];
            glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, sizeof(T), reinterpret_cast<void*>(offset[i]));
            glEnableVertexAttribArray(i);
        }
        glBindVertexArray(0);
    }

private:
    unsigned int mIndexedDrawCount;
    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;
};

}
