#include "ShaderManager.hpp"
#include "Shader.hpp"
#include "spdlog/spdlog.h"
#include <cstring>
#include <utility>

spry::ShaderManager& spry::ShaderManager::instance()
{
    static ShaderManager shaderManager;
    return shaderManager;
}

void spry::ShaderManager::loadAllShaders()
{
    loadAndGet(TEXTURED);
    loadAndGet(LINE);
    loadAndGet(FONT);
    loadAndGet(MODEL);
    loadAndGet(SHAPE);
    loadAndGet(SKYBOX);
}

const spry::Shader& spry::ShaderManager::loadAndGet(BaseShader shaderEnum)
{
    // Bounds checking
    if (shaderEnum >= ShaderManager::SHADER_MAX) {
        shaderEnum = TEXTURED;

        spdlog::error("Trying to get a shader with value({}) greater then spry::ShaderManager::SHADER_MAX({})",
            static_cast<int>(shaderEnum),
            static_cast<int>(ShaderManager::SHADER_MAX));
        spdlog::warn("Returning TEXTURED BaseShader instead");
    }

    // Check whether already loaded
    if (!instance().mShaders.contains(shaderEnum)) {
        Shader shader;
        std::string vertPath(RES_PATH);
        std::string fragPath(RES_PATH);
        vertPath.append(instance().mPaths[shaderEnum][0]);
        fragPath.append(instance().mPaths[shaderEnum][1]);

        shader
            .add(vertPath.c_str(), GL_VERTEX_SHADER)
            .add(fragPath.c_str(), GL_FRAGMENT_SHADER)
            .compile();

        instance().mShaders[shaderEnum] = std::move(shader);
    }

    return mShaders[shaderEnum];
}

const spry::Shader& spry::ShaderManager::get(spry::ShaderManager::BaseShader shaderEnum) const
{
    if (shaderEnum < ShaderManager::SHADER_MAX) {
        return mShaders.at(shaderEnum);
    } else {
        spdlog::error("Trying to get a shader with value({}) greater then spry::ShaderManager::SHADER_MAX({})",
            static_cast<int>(shaderEnum),
            static_cast<int>(ShaderManager::SHADER_MAX));
        spdlog::warn("Returning TEXTURED BaseShader instead");
        return mShaders.at(TEXTURED);
    }
}