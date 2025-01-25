#pragma once

#include "CubeMap.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "TextureRenderTarget.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

namespace spry {

class PointLight {
public:
    glm::vec3 mPosition;

    float mConstant;
    float mLinear;
    float mQuadratic;

    glm::vec3 mAmbient;
    glm::vec3 mDiffuse;
    glm::vec3 mSpecular;

    PointLight();

    void init(
        uint32_t shadowMapWidth,
        uint32_t shadowMapHeight,
        float farPlane,
        glm::vec3 mPosition,
        float constant,
        float linear,
        float quadratic,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular);

    void renderShadows(const Scene* scene) const;
    void bindUniforms(const Shader& shader, uint32_t idx) const;
    const CubeMap& getCubeMap() const;

private:
    uint32_t mShadowMapWidth { 1024 };
    uint32_t mShadowMapHeight { 1024 };
    float mFarPlane { 100.0f };
    glm::mat4 mLightProj;

    const Shader& mOmniDirShadowShader;
    CubeMap mCubeMap;
    TextureRenderTarget mPointlightShadowMapTarget;
};

}