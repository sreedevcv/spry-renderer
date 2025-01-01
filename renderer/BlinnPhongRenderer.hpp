#pragma once

#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DebugTextureViewer.hpp"
#include "DefaultScene.hpp"
#include "Entity.hpp"
#include "Materials.hpp"
#include "Plane.hpp"
#include "Shader.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"
#include "TextureRenderTarget.hpp"
#include "glm/ext/vector_float3.hpp"

#include <vector>

#define POINT_LIGHT_COUNT 4

namespace spry {

class BlinnPhongRenderer {
public:
    struct DirLight {
        glm::vec3 direction = glm::vec3(42.0f, 20.0f, 20.0f);

        glm::vec3 ambient = glm::vec3(0.3f, 0.3f, 0.3f);
        glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 specular = glm::vec3(0.9f, 0.9f, 0.9f);
    };

    struct PointLight {
        glm::vec3 position;

        float constant;
        float linear;
        float quadratic;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct SpotLight {
        glm::vec3 position;
        glm::vec3 direction;

        float innerCutOffAngle;
        float outerCutOffAngle;

        float constant;
        float linear;
        float quadratic;
    };

    BlinnPhongRenderer();

    void load(const Camera* camera);
    void addPointLight(const PointLight& pointLight);
    void setDirLight(const DirLight& dirLight);
    void setSpotLight(const SpotLight& spotLight);

    void render() const;
    void process(float delta);
    void debugView(float delta);

private:
    DirLight mDirLight;
    std::vector<PointLight> mPointLights;
    SpotLight mSpotLight;

    Shader mLightingPassShader;
    Shader mShadowPassShader;

    Texture mShadowMap;
    uint32_t mShadowMapWidth = 1024;
    uint32_t mShadowMapHeight = 1024;
    TextureRenderTarget mShadowMapTarget;
    DebugTextureViewer mTextureViewer;

    DefaultScene mDefaultScene;
    const Camera* mCamera { nullptr };

    Sphere mSphere;
    Cuboid mCuboid;
    Plane mPlane;

    int mUseBlinnPhongModel = 0;
    int mUseDirectionalLights = 1;
    int mUseSpotLights = 0;
    int mUsePointLights = 0;
    Material mCurrMaterial = *materials.at({ "yellowPlastic" });

    // Debug
    Entity3D cubeTwo { glm::vec3(3.0f, -5.0f, -5.0f) };
};

}