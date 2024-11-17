#include "IndexedVAO.hpp"

#include "spdlog/spdlog.h"

spry::IndexedVAO::IndexedVAO()
{
}

void spry::IndexedVAO::load(std::span<float> vertices, std::span<uint32_t> indices, std::span<uint32_t> format, GLenum drawtype)
{
    mVertexCount = indices.size();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (void*)vertices.data(), drawtype);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), (void*)indices.data(), GL_STATIC_DRAW);

    int stride = 0;
    for (int attribSize : format) {
        stride += attribSize;
    }

    long prevAttribSize = 0;
    for (auto i = 0ul; i < format.size(); i++) {
        int attribSize = format[i];
        glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(prevAttribSize * sizeof(float)));
        glEnableVertexAttribArray(i);
        prevAttribSize += attribSize;
    }
    glBindVertexArray(0);
}

void spry::IndexedVAO::updateMesh(std::span<float> vertices) const
{
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());
}

void spry::IndexedVAO::draw(GLenum mode) const
{
    glBindVertexArray(mVAO);
    glDrawElements(mode, mVertexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void spry::IndexedVAO::unload() const
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    spdlog::debug("Deleted IndexedVAO[{}]", mVAO);
    spdlog::debug("Deleted VBO[{}]", mVBO);
    spdlog::debug("Deleted EBO[{}]", mEBO);
}

spry::IndexedVAO& spry::IndexedVAO::operator=(IndexedVAO&& mesh)
{
    // Since we don't own the vertex data, it is better that
    // we don't delete the the buffers to preserve RAII
    // So not calling: `deleteBuffers()`

    if (this != &mesh) {
        this->mVAO = std::move(mesh.mVAO);
        this->mEBO = std::move(mesh.mEBO);
        this->mVBO = std::move(mesh.mVBO);
        this->mVertexCount = std::move(mesh.mVertexCount);
        mesh.mVAO = 0;
        mesh.mEBO = 0;
        mesh.mVBO = 0;
        mesh.mVertexCount = 0;
    }
    return *this;
}

spry::IndexedVAO::IndexedVAO(IndexedVAO&& mesh)
{
    if (this != &mesh) {
        this->mVAO = std::move(mesh.mVAO);
        this->mEBO = std::move(mesh.mEBO);
        this->mVBO = std::move(mesh.mVBO);
        this->mVertexCount = std::move(mesh.mVertexCount);

        mesh.mVAO = 0;
        mesh.mEBO = 0;
        mesh.mVBO = 0;
        mesh.mVertexCount = 0;
    }
}
