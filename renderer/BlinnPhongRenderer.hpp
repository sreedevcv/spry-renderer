#pragma once

#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DebugTextureViewer.hpp"
#include "DefaultAxes.hpp"
#include "DirLight.hpp"
#include "FullScreenQuad.hpp"
#include "Materials.hpp"
#include "Model.hpp"
#include "Plane.hpp"
#include "PointLight.hpp"
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
    Shader mLightingPassShader;
    Texture mScreenTexture;
    Texture mShapeTexture;
    
    const float mFarPlane { 150.0f };
    const uint32_t mShadowMapWidth = 1024 * 4;
    const uint32_t mShadowMapHeight = 1024 * 4;
    TextureRenderTarget mScreenTarget;
    DefaultAxes mDefaultScene;
    Camera* mCamera { nullptr };
    FullScreenQuad mScreenQuad;

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

    std::vector<PointLight> mPointLights;
    SpotLight mSpotLight;
    DirLight mDirLight;
};

}
