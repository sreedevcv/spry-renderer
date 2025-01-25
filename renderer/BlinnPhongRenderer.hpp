#pragma once

#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DebugTextureViewer.hpp"
#include "DefaultAxes.hpp"
#include "DirLight.hpp"
#include "Materials.hpp"
#include "Model.hpp"
#include "Plane.hpp"
#include "PointLight.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"
#include "glm/ext/vector_float3.hpp"
#include <vector>


#define POINT_LIGHT_COUNT 4

namespace spry {

class BlinnPhongRenderer {
public:
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
    void setSpotLight(const SpotLight& spotLight);

    void render() const;
    void process(float delta);
    void debugView(float delta);

private:
    // -46, -88, -17
    // DirLight mDirLight;
    // std::vector<PointLight> mPointLights;
    SpotLight mSpotLight;
    const float mFarPlane { 150.0f };

    Shader mLightingPassShader;
    
    Texture mShapeTexture;
    
    const uint32_t mShadowMapWidth = 1024 * 4;
    const uint32_t mShadowMapHeight = 1024 * 4;
    
    DebugTextureViewer* mTextureViewer;

    Shader mPerpectiveShadowShader;

    DefaultAxes mDefaultScene;
    Camera* mCamera { nullptr };

    Sphere mSphere;
    Cuboid mCuboid;
    Plane mPlane;
    Model mCubeModel;
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

    std::vector<PointLight> mPointLights;
    DirLight mDirLight;
};

}