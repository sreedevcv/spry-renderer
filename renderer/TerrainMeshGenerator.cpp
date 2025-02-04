#include "TerrainMeshGenerator.hpp"

#include <glad/glad.h>
#include "spdlog/spdlog.h"

spry::TerrainMeshGenerator::TerrainMeshGenerator()
{
    GLint maxTessLevel;
    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);
    spdlog::info("Max available tess level: {} ", maxTessLevel);
}

void spry::TerrainMeshGenerator::load(int32_t width, int32_t height)
{
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    uint32_t resolution = 20;
    std::vector<float> vertices;

    for (uint32_t i = 0; i < resolution; i++) {
        for (uint32_t j = 0; j < resolution; j++) {
            vertices.push_back(-width / 2.0f + width * i / float(resolution));
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + height * j / float(resolution));
            vertices.push_back(i / float(resolution));
            vertices.push_back(j / float(resolution));

            vertices.push_back(-width / 2.0f + width * (i + 1) / float(resolution));
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + height * j / float(resolution));
            vertices.push_back((i + 1) / float(resolution));
            vertices.push_back(j / float(resolution));

            vertices.push_back(-width / 2.0f + width * i / float(resolution));
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + height * (j + 1) / float(resolution));
            vertices.push_back(i / float(resolution));
            vertices.push_back((j + 1) / float(resolution));

            vertices.push_back(-width / 2.0f + width * (i + 1) / float(resolution));
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + height * (j + 1) / float(resolution));
            vertices.push_back((i + 1) / float(resolution));
            vertices.push_back((j + 1) / float(resolution));
        }
    }

    std::array<uint32_t, 2> format { 3, 2 };
    mVAO.load(vertices, format, resolution * resolution * 4, GL_STATIC_DRAW);

    mTesselationShader
        .add(RES_PATH "shaders/HeightMap.vert", GL_VERTEX_SHADER)
        .add(RES_PATH "shaders/HeightMap.frag", GL_FRAGMENT_SHADER)
        .add(RES_PATH "shaders/HeightMap.tessctl.glsl", GL_TESS_CONTROL_SHADER)
        .add(RES_PATH "shaders/HeightMap.tesseval.glsl", GL_TESS_EVALUATION_SHADER)
        .compile();
}

void spry::TerrainMeshGenerator::draw(const Texture& heightMap, const Camera& camera) const
{
    const auto model = glm::mat4 { 1.0f };

    mTesselationShader.bind();
    mTesselationShader.setUniformMatrix("proj", camera.getProjectionMatrix());
    mTesselationShader.setUniformMatrix("view", camera.getViewMatrix());
    mTesselationShader.setUniformMatrix("model", model);
    mTesselationShader.setUniformInt("heightMap", 0);

    heightMap.bind(0);
    mVAO.draw(GL_PATCHES);
}