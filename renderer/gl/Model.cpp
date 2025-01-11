#include "Model.hpp"

#include "Mesh.hpp"
#include "Texture.hpp"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/types.h>
#include <glm/ext/vector_float3.hpp>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <iterator>
#include <print>
#include <string>
#include <utility>
#include <vector>

void spry::Model::load(const char* path)
{
    spdlog::info("Loading model: {}", path);

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_CalcTangentSpace //
            | aiProcess_Triangulate //
            | aiProcess_JoinIdenticalVertices //
            | aiProcess_SortByPType //
            | aiProcess_GenSmoothNormals //
            | aiProcess_FlipUVs);

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("Failed to load model: {}", path);
        spdlog::error("Assimp: {}", importer.GetErrorString());
        return;
    }

    auto pathStr = std::string(path);
    mDirectory = pathStr.substr(0, pathStr.find_last_of('/'));

    processNode(scene->mRootNode, scene);

    for (auto& mesh : mMeshes) {
        mesh.load(&mTextures);
    }

    spdlog::info("Completed loading model with {} mesh(s) and {} texture(s)", mMeshes.size(), mLoadedTextures.size());
}

void spry::Model::processNode(aiNode* node, const aiScene* scene)
{
    spdlog::debug("Loading meshes of node with {} meshes and {} children", node->mNumMeshes, node->mNumChildren);
    for (auto i = 0ul; i < node->mNumMeshes; i++) {
        const int meshIndex = node->mMeshes[i];
        aiMesh* mesh = scene->mMeshes[meshIndex];
        mMeshes.push_back(processMesh(mesh, scene));
    }

    for (auto i = 0ul; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

spry::Mesh spry::Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> textures;

    // Get the vertices
    for (auto i = 0ul; i < mesh->mNumVertices; i++) {
        auto position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);
        auto normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z);
        auto texCoord = mesh->mTextureCoords[0]
            ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
            : glm::vec2(0.0f);

        vertices.push_back(Mesh::Vertex {
            .position = std::move(position),
            .normal = std::move(normal),
            .texCoord = std::move(texCoord),
        });
    }

    // Get the indices
    for (auto i = 0ul; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];

        for (auto j = 0ul; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    //  AI_MATKEY_COLOR_DIFFUSE
    //  AI_MATKEY_COLOR_AMBIENT
    //  AI_MATKEY_COLOR_SPECULAR
    //  AI_MATKEY_COLOR_EMISSIVE
    //  AI_MATKEY_COLOR_TRANSPARENT
    //  AI_MATKEY_COLOR_REFLECTIVE

    // Get materials
    if (mesh->mMaterialIndex >= 0) {
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
        auto specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
        auto normalMaps = loadMaterialTexture(material, aiTextureType_NORMALS, "texture_normal");
        auto heightMaps = loadMaterialTexture(material, aiTextureType_HEIGHT, "texture_height");

        textures.insert(textures.end(), std::make_move_iterator(diffuseMaps.begin()), std::make_move_iterator(diffuseMaps.end()));
        textures.insert(textures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));
        textures.insert(textures.end(), std::make_move_iterator(normalMaps.begin()), std::make_move_iterator(normalMaps.end()));
        textures.insert(textures.end(), std::make_move_iterator(heightMaps.begin()), std::make_move_iterator(heightMaps.end()));
    }

    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

// Are we just randomly loading materials for every node instead of chcking typeName
// Complare with learnOpengl's implementation
std::vector<uint32_t> spry::Model::loadMaterialTexture(
    const aiMaterial* material,
    aiTextureType type,
    const char* typeName)
{
    std::vector<uint32_t> textures;

    for (auto i = 0ul; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);
        auto pathStr = std::string(str.C_Str());

        if (!mLoadedTextures.contains(pathStr)) {
            Texture texture;
            std::string path = mDirectory + '/' + pathStr;

            texture.create()
                .setFilterMode(GL_LINEAR)
                .setWrapMode(GL_CLAMP_TO_EDGE)
                .load(path.c_str());

            mTextures.push_back(
                Mesh::NamedTexture {
                    std::move(texture),
                    std::string(typeName) });

            textures.push_back(mTextures.size() - 1);
            mLoadedTextures.insert(std::move(pathStr));
        }
    }

    return textures;
}

void spry::Model::draw() const
{
    for (const auto& mesh : mMeshes) {

        for (auto texCount = 0ul; texCount < mesh.getTexturesIndices().size(); texCount++) {

            const auto index = mesh.getTexturesIndices()[texCount];
            const auto& [texture, name] = mTextures[index];

            if (name == "texture_diffuse") {
                texture.bind(texCount);
            }
            // TODO::Implement for other textures
        }
        mesh.draw();
    }
}

spry::Model::Model(Model&& model)
{
    mMeshes = std::move(model.mMeshes);
    mTextures = std::move(model.mTextures);
    mDirectory = std::move(model.mDirectory);
    mLoadedTextures = std::move(model.mLoadedTextures);
}

spry::Model& spry::Model::operator=(Model&& model)
{
    if (this != &model) {
        mMeshes = std::move(model.mMeshes);
        mTextures = std::move(model.mTextures);
        mDirectory = std::move(model.mDirectory);
        mLoadedTextures = std::move(model.mLoadedTextures);
    }

    return *this;
}
