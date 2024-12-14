#include "VAO.hpp"

#include <cstdint>
#include <spdlog/spdlog.h>

spry::VAO::VAO()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
}

void spry::VAO::load(std::span<float> vertices, std::span<uint32_t> format, uint32_t vertexCount, GLenum drawtype)
{
    void* data = (void*)vertices.data();
    if (drawtype == GL_DYNAMIC_DRAW) {
        data = nullptr;
    }

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), data, drawtype);

    int stride = 0;
    for (int attribSize : format) {
        stride += attribSize;
    }
    
    mVertexCount = vertexCount;

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
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());
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
