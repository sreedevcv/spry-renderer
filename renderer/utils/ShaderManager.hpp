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
        SHAPE,
        SKYBOX,
        POINT_LIGHT_SHADOW_MAP,
        SHADER_MAX,
    };

    static ShaderManager& instance();
    const Shader& get(BaseShader shaderEnum) const;
    void loadAllShaders();
    const Shader& loadAndGet(BaseShader shaderEnum);

private:
    std::unordered_map<BaseShader, Shader> mShaders;
    const char* mPaths[7][3] = {
        { "shaders/Textured.vert", "shaders/Textured.frag", "" },
        { "shaders/Line.vert", "shaders/Line.frag", "" },
        { "shaders/Font.vert", "shaders/Font.frag", "" },
        { "shaders/Model.vert", "shaders/Model.frag", "" },
        { "shaders/Shape.vert", "shaders/Shape.frag", "" },
        { "shaders/Skybox.vert", "shaders/Skybox.frag", "" },
        { "shaders/OmniDirectionalShadow.vert", "shaders/OmniDirectionalShadow.frag", "shaders/OmniDirectionalShadow.geom" },
    };
};

}