#include "ShaderManager.hpp"
#include "Shader.hpp"
#include "spdlog/spdlog.h"
#include <array>
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
    loadAndGet(POINT_LIGHT_SHADOW_MAP);
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

    constexpr std::array typeArray {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER,
        GL_GEOMETRY_SHADER,
    };

    // Check whether already loaded
    if (!instance().mShaders.contains(shaderEnum)) {
        Shader shader;

        for (int i = 0; i < 3; i++) {
            // If not empty 
            if (mPaths[shaderEnum][i][0] != '\0') {
                std::string path(RES_PATH);
                path.append(mPaths[shaderEnum][i]);
                shader = std::move(shader.add(path.c_str(), typeArray[i]));
            }
        }

        shader.compile();
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