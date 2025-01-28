#pragma once

#include "Scene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureRenderTarget.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include <cstdint>

namespace spry {

class DirLight {
public:
    glm::vec3 mDirection = glm::vec3(-42.0f, -20.0f, -20.0f);
    glm::vec3 mAmbient = glm::vec3(0.3f, 0.3f, 0.3f);
    glm::vec3 mDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 mSpecular = glm::vec3(0.9f, 0.9f, 0.9f);

    DirLight();

    void init(glm::vec3 direction,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        uint32_t shadowMapWidth,
        uint32_t shadowMapHeight);

    glm::mat4 renderShadows(const Scene* scene) const;
    const Texture& getDepthMap() const;

private:
    const Shader& mShadowPassShader;
    Texture mShadowMap;
    TextureRenderTarget mRenderTarget;

    uint32_t mShadowMapWidth { 1024 };
    uint32_t mShadowMapHeight { 1024 };
};

}
