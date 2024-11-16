#include "VAO.hpp"

#include <cstdint>
#include <spdlog/spdlog.h>

spry::VAO::VAO()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
}

void spry::VAO::load(std::span<float> vertices, std::span<uint32_t> format, GLenum drawtype)
{
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (void*)vertices.data(), drawtype);

    int stride = 0;
    for (int attribSize : format) {
        stride += attribSize;
    }
    mVertexCount = (vertices.size() * sizeof(float)) / stride;

    long prevAttribSize = 0;
    for (auto i = 0ul; i < format.size(); i++) {
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
    glDrawArrays(mode, 0, mVertexCount);
    glBindVertexArray(0);
}

void spry::VAO::updateMesh(std::span<float> vertices) const
{
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size(), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void spry::VAO::unload() const
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    spdlog::debug("Deleted VAO[{}]", mVAO);
    spdlog::debug("Deleted VBO[{}]", mVBO);
}

spry::VAO& spry::VAO::operator=(VAO&& mesh)
{
    // Since we don't own the vertex data, it is better that
    // we don't delete the the buffers to preserve RAII
    // So not calling: `deleteBuffers()`

    if (this != &mesh) {
        this->mVAO = std::move(mesh.mVAO);
        this->mVBO = std::move(mesh.mVBO);
        this->mVertexCount = std::move(mesh.mVertexCount);
        mesh.mVAO = 0;
        mesh.mVBO = 0;
        mesh.mVertexCount = 0;
    }
    return *this;
}

spry::VAO::VAO(VAO&& mesh)
{
    if (this != &mesh) {
        this->mVAO = std::move(mesh.mVAO);
        this->mVBO = std::move(mesh.mVBO);
        this->mVertexCount = std::move(mesh.mVertexCount);

        mesh.mVAO = 0;
        mesh.mVBO = 0;
        mesh.mVertexCount = 0;
    }
}
