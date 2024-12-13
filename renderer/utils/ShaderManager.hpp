#pragma once

#include "Shader.hpp"
#include <unordered_map>

namespace spry {

class ShaderManager {
public:
    enum BaseShader {
        TEXTURED,
        LINE,
        FONT,
        MODEL,
        SHADER_MAX,
    };

    static ShaderManager& instance();
    const Shader& get(BaseShader shaderEnum);
    void loadAllShaders();
    const Shader& loadAndGet(BaseShader shaderEnum);

private:
    std::unordered_map<BaseShader, Shader> mShaders;
    const char* mPaths[4][2] = {
        { "shaders/Textured.vert", "shaders/Textured.frag" },
        { "shaders/Line.vert", "shaders/Line.frag" },
        { "shaders/Font.vert", "shaders/Font.frag" },
        { "shaders/Model.vert", "shaders/Model.frag" },
    };
};

}