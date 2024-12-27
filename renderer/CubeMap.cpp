#include "CubeMap.hpp"

#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "glm/common.hpp"
#include "glm/exponential.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "glad/glad.h"

#include <cmath>
#include <cstdint>
#include <numbers>
#include <sys/types.h>
#include <vector>

#define CUBE_MAP_INDEX_POS_X 0
#define CUBE_MAP_INDEX_NEG_X 1
#define CUBE_MAP_INDEX_POS_Y 2
#define CUBE_MAP_INDEX_NEG_Y 3
#define CUBE_MAP_INDEX_POS_Z 4
#define CUBE_MAP_INDEX_NEG_Z 5

spry::CubeMap::CubeMap()
    : mShader { ShaderManager::instance().loadAndGet(ShaderManager::SKYBOX) }
{
}

spry::CubeMap::~CubeMap()
{
}

spry::CubeMap::CubeMap(CubeMap&& cubeMap)
    : mShader { ShaderManager::instance().loadAndGet(ShaderManager::SKYBOX) }
{
    if (this != &cubeMap) {
        mTexID = cubeMap.mTexID;
        cubeMap.mTexID = 0;
    }
}

spry::CubeMap& spry::CubeMap::operator=(CubeMap&& cubeMap)
// : mShader { ShaderManager::instance().loadAndGet(ShaderManager::FONT) }
{
    if (this != &cubeMap) {
        mTexID = cubeMap.mTexID;
        cubeMap.mTexID = 0;
    }
    return *this;
}

static glm::vec3 faceCoordsToXYZ(uint32_t x, uint32_t y, uint32_t faceID, uint32_t faceSize)
{
    const float a = 2.0f * float(x) / faceSize;
    const float b = 2.0f * float(y) / faceSize;

    switch (faceID) {
    case CUBE_MAP_INDEX_POS_X:
        return glm::vec3(a - 1.0f, 1.0f, 1.0f - b);

    case CUBE_MAP_INDEX_NEG_X:
        return glm::vec3(1.0f - a, -1.0f, 1.0f - b);

    case CUBE_MAP_INDEX_POS_Y:
        return glm::vec3(1.0f - b, a - 1.0f, 1.0f);

    case CUBE_MAP_INDEX_NEG_Y:
        return glm::vec3(b - 1.0f, a - 1.0f, -1.0f);

    case CUBE_MAP_INDEX_POS_Z:
        return glm::vec3(-1.0f, a - 1.0f, 1.0f - b);

    case CUBE_MAP_INDEX_NEG_Z:
        return glm::vec3(1.0f, 1.0f - a, 1.0f - b);
    default:
        return glm::vec3(0.0f);
    }
}

static glm::vec4 getPixels(const float* image, int width, int componentCount, int x, int y)
{
    int idx = (width * y + x) * componentCount;

    if (componentCount == 3) {
        return glm::vec4(image[idx], image[idx + 1], image[idx + 2], 1.0f);
    } else {
        spdlog::warn("Only 3 channels supported for cubemaps");
        return glm::vec4(image[idx], image[idx + 1], image[idx + 2], image[idx + 3]);
    }
}

std::vector<std::vector<float>> spry::CubeMap::createCubeMapImages(uint32_t width, uint32_t height, int componentCount, const float* image) const
{
    // Map image to 6 faces
    const uint32_t faceSize = width / 4;
    std::vector<std::vector<float>> faces(numFaces);

    for (int i = 0; i < numFaces; i++) {
        faces[i].reserve(faceSize * faceSize * componentCount);
    }

    const uint32_t maxWidth = width - 1;
    const uint32_t maxHeight = height - 1;

    for (uint32_t face = 0; face < numFaces; face++) {
        for (uint32_t y = 0; y < faceSize; y++) {
            for (uint32_t x = 0; x < faceSize; x++) {
                const auto pos = faceCoordsToXYZ(x, y, face, faceSize);
                const float R = glm::sqrt(pos.x * pos.x + pos.y * pos.y);
                const float phi = atan2f(pos.y, pos.x);
                const float theta = atan2f(pos.z, R);

                // Calcuate texture coords
                const float u = (phi + std::numbers::pi) / (2.0f * std::numbers::pi);
                const float v = (std::numbers::pi / 2.0f - theta) / (std::numbers::pi);

                // Scale texture coords by image size
                const float U = u * width;
                const float V = v * height;

                // 4 samples for bilinear interpolation
                const int u1 = glm::clamp(int(floor(U)), 0, int(maxWidth));
                const int v1 = glm::clamp(int(floor(V)), 0, int(maxHeight));
                const int u2 = glm::clamp(u1 + 1, 0, int(maxWidth));
                const int v2 = glm::clamp(v1 + 1, 0, int(maxHeight));

                // calculate fractional parts
                const float s = U - u1;
                const float t = V - v1;

                // Fetch 4 samples
                const glm::vec4 bottomLeft = getPixels(image, width, componentCount, u1, v1);
                const glm::vec4 bottomRight = getPixels(image, width, componentCount, u2, v1);
                const glm::vec4 topLeft = getPixels(image, width, componentCount, u1, v2);
                const glm::vec4 topRight = getPixels(image, width, componentCount, u2, v2);

                // Interpolation
                const glm::vec4 color = bottomLeft * (1 - s) * (1 - t) + //
                    bottomRight * s * (1 - t) + //
                    topLeft * (1 - s) * t + //
                    topRight * s * t;

                const int idx = (faceSize * y + x) * componentCount;
                faces[face][idx + 0] = color.r;
                faces[face][idx + 1] = color.g;
                faces[face][idx + 2] = color.b;
                // faces[face][idx + 3] = color.a;

                // faces[face].push_back(color.r);
                // faces[face].push_back(color.g);
                // faces[face].push_back(color.b);
                // faces[face].push_back(color.a);
            }
        }
    }

    return faces;
}

void spry::CubeMap::load(std::filesystem::path path)
{
    int width;
    int height;
    int componentCount;

    const float* image = stbi_loadf(path.c_str(), &width, &height, &componentCount, 3);

    if (image == nullptr) {
        spdlog::error("Failed to load cubemap image: {}", path.c_str());
        return;
    } else if (componentCount != 3) {
        spdlog::error("Expected cubemap image: {} to have only 3 color channels, but found: {}", path.c_str(), componentCount);
        stbi_image_free((void*)image);
        return;
    }

    const uint32_t faceSize = width / 4;
    const auto faces = createCubeMapImages(width, height, componentCount, image);

    stbi_image_free((void*)image);

    // load cube map
    loadTextures(faces, faceSize, faceSize);
}

void spry::CubeMap::loadTextures(const std::vector<std::vector<float>>& faceImages, uint32_t width, uint32_t height)
{
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mTexID);
    glTextureParameteri(mTexID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mTexID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mTexID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mTexID, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(mTexID, GL_TEXTURE_MAX_LEVEL, 0);
    glTextureParameteri(mTexID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mTexID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(mTexID, 1, GL_RGB32F, width, height);

    for (int i = 0; i < numFaces; i++) {
        const void* data = faceImages[i].data();
        glTextureSubImage3D(mTexID,
            0,
            0,
            0,
            i,
            width,
            height,
            1,
            GL_RGB,
            GL_FLOAT,
            data);
    }
}

void spry::CubeMap::bind(uint32_t textureUnit) const
{
    glBindTextures(GL_TEXTURE0 + textureUnit, 1, &mTexID);
}

void spry::CubeMap::draw() const
{
    GLint OldCullFaceMode;
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    bind(0);
    mShader.bind();
    mCube.draw();

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
}

const spry::Shader& spry::CubeMap::getShader() const
{
    return mShader;
}