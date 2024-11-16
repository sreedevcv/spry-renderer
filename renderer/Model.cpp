#include "Model.hpp"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <print>
#include <spdlog/spdlog.h>

void spry::Model::load(const char* path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_CalcTangentSpace //
            | aiProcess_Triangulate //
            | aiProcess_JoinIdenticalVertices //
            | aiProcess_SortByPType //
    );

    if  (scene == nullptr) {
        spdlog::error("Failed to load model: {}", path);
        return;
    }

    
}