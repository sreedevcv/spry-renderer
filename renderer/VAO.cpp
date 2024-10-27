#include "VAO.hpp"
#include <cstdint>

void spry::VAO::load(std::span<float> vertices, std::span<uint32_t> indices, std::span<uint32_t> format)
{
    if (mDataLoaded) {
        unload();
    }

    mVertexCount = indices.size();
    mDataLoaded = true;
    mWithIndexArray = true;

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (void*)vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), (void*)indices.data(), GL_STATIC_DRAW);

    int stride = 0;
    for (int attribSize : format) {
        stride += attribSize;
    }

    long prevAttribSize = 0;
    for (int i = 0; i < format.size(); i++) {
        int attribSize = format[i];
        glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(prevAttribSize * sizeof(float)));
        glEnableVertexAttribArray(i);
        prevAttribSize += attribSize;
    }
    glBindVertexArray(0);
}

void spry::VAO::load(std::span<float> vertices, std::span<uint32_t> format)
{
    if (mDataLoaded) {
        unload();
    }

    mDataLoaded = true;
    mWithIndexArray = false;

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (void*)vertices.data(), GL_STATIC_DRAW);

    int stride = 0;
    for (int attribSize : format) {
        stride += attribSize;
    }
    mVertexCount = (vertices.size() * sizeof(float)) / stride;

    long prevAttribSize = 0;
    for (int i = 0; i < format.size(); i++) {
        int attribSize = format[i];
        glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(prevAttribSize * sizeof(float)));
        glEnableVertexAttribArray(i);
        prevAttribSize += attribSize;
    }
    glBindVertexArray(0);
}

void spry::VAO::draw(GLenum mode) const
{
    glBindVertexArray(mVAO);
    if (mWithIndexArray) {
        glDrawElements(mode, mVertexCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(mode, 0, mVertexCount);
    }
    glBindVertexArray(0);
}

void spry::VAO::unload() const
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);

    if (mWithIndexArray) {
        glDeleteBuffers(1, &mEBO);
    }
}

spry::VAO& spry::VAO::operator=(VAO& mesh)
{
    if (this == &mesh) {
        return *this;
    }

    // Since we don't own the vertex data, it is better that
    // we don't delete the the buffers to preserve RAII
    // So not calling: `deleteBuffers()`

    this->mVAO = mesh.mVAO;
    this->mEBO = mesh.mEBO;
    this->mVBO = mesh.mVBO;
    this->mVertexCount = mesh.mVertexCount;
    return *this;
}

spry::VAO& spry::VAO::operator=(VAO&& mesh)
{
    if (this == &mesh) {
        return *this;
    }

    // Since we don't own the vertex data, it is better that
    // we don't delete the the buffers to preserve RAII
    // So not calling: `deleteBuffers()`

    this->mVAO = mesh.mVAO;
    this->mEBO = mesh.mEBO;
    this->mVBO = mesh.mVBO;
    this->mVertexCount = mesh.mVertexCount;
    return *this;
}

spry::VAO::VAO(VAO& mesh)
{
    this->mVAO = mesh.mVAO;
    this->mEBO = mesh.mEBO;
    this->mVBO = mesh.mVBO;
    this->mVertexCount = mesh.mVertexCount;
}

spry::VAO::VAO(VAO&& mesh)
{
    this->mVAO = mesh.mVAO;
    this->mEBO = mesh.mEBO;
    this->mVBO = mesh.mVBO;
    this->mVertexCount = mesh.mVertexCount;
}
