#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "Drawable.hpp"
#include "Mesh.hpp"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"

namespace spry {

class Model: public Drawable {
public:
    Model() = default;
    virtual ~Model() = default;

    Model(Model&& model);
    Model& operator=(Model&& model);
    Model(const Model& model) = delete;
    Model& operator=(const Model& model) = delete;

    void load(const char* path);
    void draw() const override;

    bool loggedOnce = false;

private:
    std::string mDirectory;
    std::vector<Mesh> mMeshes;
    std::vector<Mesh::NamedTexture> mTextures;
    std::unordered_set<std::string> mLoadedTextures;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<uint32_t> loadMaterialTexture(const aiMaterial* material, aiTextureType type, const char* typeName);
};

}
