#include "Sphere.hpp"

#include "IndexedVAO.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/trigonometric.hpp"

#include <array>
#include <cstdint>
#include <sys/types.h>
#include <vector>

void spry::Sphere::load(uint32_t width, uint32_t height)
{
    // std::vector<float> vertices;
    // vertices.reserve((height + 1) * (width + 1) * (3 + 3 + 2));
    // const float PI = glm::pi<float>();

    // for (uint32_t i = 0; i < height + 1; i++) {
    //     const float theta = float(i) * PI / float(height);

    //     for (uint32_t j = 0; j < width + 1; j++) {
    //         // Vertices
    //         const float phi = 2.0f * PI * float(j) / float(width);
    //         const float x = glm::cos(phi) * glm::sin(theta);
    //         const float y = glm::cos(theta);
    //         const float z = glm::sin(phi) * glm::sin(theta);

    //         vertices.push_back(x);
    //         vertices.push_back(y);
    //         vertices.push_back(z);

    //         // Normals
    //         vertices.push_back(x);
    //         vertices.push_back(y);
    //         vertices.push_back(z);

    //         // Tex coords
    //         const float u = 1 - (float(j) / width);
    //         const float v = 1 - (float(i) / height);
    //         vertices.push_back(u);
    //         vertices.push_back(v);
    //     }
    // }

    // std::vector<uint32_t> indices;
    // indices.reserve(height * width * 6);

    // for (int i = 0; i < height; i++) {
    //     for (uint32_t j = 0; j < width; j++) {
    //         const uint32_t one = (i * (width + 1)) + j;
    //         const uint32_t two = one + width + 1;

    //         indices.push_back(one);
    //         indices.push_back(two);
    //         indices.push_back(one + 1);

    //         indices.push_back(two);
    //         indices.push_back(two + 1);
    //         indices.push_back(one + 1);
    //     }
    // }

    const uint32_t stackCount = height;
    const uint32_t sectorCount = width;
    const float radius = 1.0f;
    const float PI = glm::pi<float>();
    std::vector<float> vertices;

    float x, y, z, xy; // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal
    float s, t; // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (uint32_t i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle); // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for (uint32_t j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }

    std::vector<uint32_t> indices;

    uint32_t k1, k2;
    for (uint32_t i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1); // beginning of current stack
        k2 = k1 + sectorCount + 1; // beginning of next stack

        for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    std::array<uint32_t, 3> format = { 3, 3, 2 };

    mVao.load(vertices, indices, format, GL_STATIC_DRAW);
}

void spry::Sphere::draw() const
{
    mVao.draw(GL_TRIANGLES);
}