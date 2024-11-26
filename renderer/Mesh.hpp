#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "IndexedVAO.hpp"
#include "Texture.hpp"

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

namespace spry {

class Mesh {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    using NamedTexture = std::pair<Texture, std::string>;

    Mesh(std::vector<Vertex>& vertices,
        std::vector<uint32_t>& indices,
        std::vector<NamedTexture>& textures);

    Mesh(Mesh&& mesh);
    Mesh& operator=(Mesh&& mesh);
    Mesh(const Mesh& mesh) = delete;
    Mesh& operator=(const Mesh& mesh) = delete;

    void draw() const;
    void load();

private:
    std::vector<Vertex> mVertices;
    std::vector<uint32_t> mIndices;
    std::vector<NamedTexture> mTextures;
    IndexedVAO mVao;
};

}
