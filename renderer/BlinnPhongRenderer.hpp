#pragma once

#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DebugTextureViewer.hpp"
#include "DefaultAxes.hpp"
#include "Materials.hpp"
#include "Plane.hpp"
#include "Scene.hpp"
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
        glm::vec3 direction = glm::vec3(-42.0f, -20.0f, -20.0f);

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

    void load(Camera* camera);
    void addPointLight(const PointLight& pointLight);
    void setDirLight(const DirLight& dirLight);
    void setSpotLight(const SpotLight& spotLight);

    void render() const;
    void process(float delta);
    void debugView(float delta);

private:
    // -46, -88, -17
    DirLight mDirLight;
    std::vector<PointLight> mPointLights;
    SpotLight mSpotLight;

    Shader mLightingPassShader;
    Shader mShadowPassShader;
    Texture mShapeTexture;
    Texture mShadowMap;
    uint32_t mShadowMapWidth = 1024 * 4;
    uint32_t mShadowMapHeight = 1024 * 4;
    TextureRenderTarget mShadowMapTarget;
    DebugTextureViewer mTextureViewer;

    DefaultAxes mDefaultScene;
    Camera* mCamera { nullptr };

    Sphere mSphere;
    Cuboid mCuboid;
    Plane mPlane;
    Scene* mSphereScene {};

    Material mCurrMaterial = *materials.at({ "gold" });

    // Debug
    int mUseBlinnPhongModel = 1;
    int mUseDirectionalLights = 1;
    int mUseSpotLights = 0;
    int mUsePointLights = 0;
    int mShadowSampling = 1;
    // float oleft = -10.0f;
    // float oright = 10.0f;
    // float otop = 10.0f;
    // float obottom = -10.0f;
    float oleft = -50.0f;
    float oright = 50.0f;
    float otop = 50.0f;
    float obottom = -50.0f;

    float ofar = 100.0f;
    float onear = 0.1f;
};

}