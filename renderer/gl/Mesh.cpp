#include "Mesh.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

spry::Mesh::Mesh(std::vector<Vertex>&& vertices,
    std::vector<uint32_t>&& indices,
    std::vector<uint32_t>&& textures)
    : mVertices { std::move(vertices) }
    , mIndices { std::move(indices) }
    , mTextures { std::move(textures) }
{
}

void spry::Mesh::load(std::vector<NamedTexture>* textureReference)
{
    std::array<uint32_t, 3> sizes = { 3, 3, 2 };
    std::array<uint32_t, 3> offset = {
        offsetof(Vertex, position),
        offsetof(Vertex, normal),
        offsetof(Vertex, texCoord)
    };
    mVao.load<Mesh::Vertex>(mVertices, mIndices, sizes, offset, GL_STATIC_DRAW);
    mTextureReference = textureReference;
}

// FIXME::Bind the correct textures before drawing
void spry::Mesh::draw() const
{
    mVao.draw();
}

const std::vector<uint32_t>& spry::Mesh::getTexturesIndices() const
{
    return mTextures;
}

spry::Mesh::Mesh(Mesh&& mesh)
{
    mTextureReference = std::move(mesh.mTextureReference);
    mVertices = std::move(mesh.mVertices);
    mIndices = std::move(mesh.mIndices);
    mTextures = std::move(mesh.mTextures);
    mVao = std::move(mVao);
}

spry::Mesh& spry::Mesh::operator=(Mesh&& mesh)
{
    if (this != &mesh) {
        mTextureReference = std::move(mesh.mTextureReference);
        mVertices = std::move(mesh.mVertices);
        mIndices = std::move(mesh.mIndices);
        mTextures = std::move(mesh.mTextures);
        mVao = std::move(mVao);
    }
    return *this;
}
