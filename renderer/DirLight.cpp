#include "DirLight.hpp"
#include "glm/ext/matrix_float4x4.hpp"

void spry::DirLight::init(glm::vec3 direction,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    uint32_t shadowMapWidth,
    uint32_t shadowMapHeight)
{
    mDirection = direction;
    mAmbient = ambient;
    mDiffuse = diffuse;
    mSpecular = specular;
    mShadowMapWidth = shadowMapWidth;
    mShadowMapHeight = shadowMapHeight;

    mShadowPassShader
        .add(RES_PATH "shaders/Shadow.vert", GL_VERTEX_SHADER)
        .add(RES_PATH "shaders/Shadow.frag", GL_FRAGMENT_SHADER)
        .compile();

    std::array borderColors { 1.0f, 1.0f, 1.0f, 1.0f };

    mShadowMap
        .create()
        .setWrapMode(GL_CLAMP_TO_BORDER)
        .setFilterMode(GL_LINEAR)
        .setBorderColor(borderColors)
        .setCompareModeAndFunc(GL_COMPARE_REF_TO_TEXTURE, GL_LEQUAL)
        .load(nullptr, mShadowMapWidth, mShadowMapHeight, GL_DEPTH_COMPONENT, GL_FLOAT);

    mRenderTarget.load();
    mRenderTarget.attachTextureDepth(mShadowMap);
}

glm::mat4 spry::DirLight::renderShadows(const Scene* scene) const
{
    float oleft = -50.0f;
    float oright = 50.0f;
    float otop = 50.0f;
    float obottom = -50.0f;

    float ofar = 100.0f;
    float onear = 0.001f;

    /* NOTE::A plane has only a single face. Using glCullFace(GL_FRONT) would completely
           remove it So ideally, it should not be used to rendering the plane */
    mRenderTarget.bind();

    // glCullFace(GL_FRONT);
    glViewport(0, 0, mShadowMapWidth, mShadowMapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);

    const auto lightProj = glm::ortho(
        oleft,
        oright,
        obottom,
        otop,
        onear,
        ofar);
    // NOTE::Should eye for ortho projection be -direction or +direction??
    auto lighView = glm::lookAt(
        -mDirection,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    const auto lightViewProj = lightProj * lighView;

    mShadowPassShader.bind();
    mShadowPassShader.setUniformMatrix("lightViewProj", lightViewProj);

    auto model = glm::mat4(1.0f);
    scene->draw(model, mShadowPassShader);

    // glCullFace(GL_BACK);

    mRenderTarget.unbind();

    return lightViewProj;
}

const spry::Texture& spry::DirLight::getDepthMap() const
{
    return mShadowMap;
}