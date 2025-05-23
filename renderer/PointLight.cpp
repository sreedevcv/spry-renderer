#include "PointLight.hpp"
#include "ShaderManager.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <format>
#include <limits>


spry::PointLight::PointLight()
    : mOmniDirShadowShader(ShaderManager::instance().loadAndGet(ShaderManager::POINT_LIGHT_SHADOW_MAP))
{
}

void spry::PointLight::init(
    uint32_t shadowMapWidth,
    uint32_t shadowMapHeight,
    float farPlane,
    glm::vec3 position,
    float constant,
    float linear,
    float quadratic,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular)
{
    mShadowMapWidth = shadowMapWidth;
    mShadowMapHeight = shadowMapHeight;
    mFarPlane = farPlane;
    mPosition = position;
    mConstant = constant;
    mLinear = linear;
    mQuadratic = quadratic;
    mAmbient = ambient;
    mDiffuse = diffuse;
    mSpecular = specular;
    mLightProj = glm::perspective(
        glm::radians(90.0f),
        float(mShadowMapWidth) / float(mShadowMapHeight),
        1.0f,
        mFarPlane);

    mCubeMap
        .create()
        .setFilterMode(GL_NEAREST)
        .setWrapMode(GL_CLAMP_TO_EDGE)
        .load(nullptr, mShadowMapWidth, mShadowMapHeight, GL_DEPTH_COMPONENT, GL_FLOAT);

    mPointlightShadowMapTarget.load();
    mPointlightShadowMapTarget.attachTextureDepth(mCubeMap);
}

void spry::PointLight::renderShadows(const Scene* scene) const
{
    mPointlightShadowMapTarget.bind();
    {
        constexpr float floatMax = std::numeric_limits<float>().max();
        
        glViewport(0, 0, mShadowMapWidth, mShadowMapHeight);
        glClearColor(floatMax, floatMax, floatMax, floatMax); 
        glClear(GL_DEPTH_BUFFER_BIT);

        const std::array<glm::mat4, CubeMap::faceCount> transforms {
            mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
            mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
            mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
            mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
            mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
            mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)),
        };

        auto model = glm::mat4(1.0f);

        mOmniDirShadowShader.bind();
        mOmniDirShadowShader.setUniformMatrix("model", model);
        mOmniDirShadowShader.setUniformMatrix("shadowMatrices", transforms.data(), transforms.size());
        mOmniDirShadowShader.setUniformVec("lightPosition", mPosition);
        mOmniDirShadowShader.setUniformFloat("farPlane", mFarPlane);
        scene->draw(model, mOmniDirShadowShader);
    }
    mPointlightShadowMapTarget.unbind();
}

void spry::PointLight::bindUniforms(const Shader& shader, uint32_t idx) const
{
    shader.bind();
    shader.setUniformVec(std::format("pointLights[{}].position", idx).c_str(), mPosition);
    shader.setUniformFloat(std::format("pointLights[{}].constant", idx).c_str(), mConstant);
    shader.setUniformFloat(std::format("pointLights[{}].linear", idx).c_str(), mLinear);
    shader.setUniformFloat(std::format("pointLights[{}].quadratic", idx).c_str(), mQuadratic);
    shader.setUniformVec(std::format("pointLights[{}].ambient", idx).c_str(), mAmbient);
    shader.setUniformVec(std::format("pointLights[{}].diffuse", idx).c_str(), mDiffuse);
    shader.setUniformVec(std::format("pointLights[{}].specular", idx).c_str(), mSpecular);
}

const spry::CubeMap& spry::PointLight::getCubeMap() const
{
    return mCubeMap;
}