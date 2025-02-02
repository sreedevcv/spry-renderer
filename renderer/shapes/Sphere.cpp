#include "Sphere.hpp"

#include "IndexedVAO.hpp"
#include "glm/ext/scalar_constants.hpp"

#include <array>
#include <cstdint>
#include <vector>
#include <cmath>

void spry::Sphere::load(uint32_t width, uint32_t height)
{
    const uint32_t stackCount = height;
    const uint32_t sectorCount = width;
    const float radius = 1.0f;
    const float PI = glm::pi<float>();
    const float lengthInv = 1.0f / radius; // vertex normal
    const float sectorStep = 2 * PI / sectorCount;
    const float stackStep = PI / stackCount;
    std::vector<float> vertices;

    for (uint32_t i = 0; i <= stackCount; ++i) {
        const float stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        const float xy = radius * cosf(stackAngle); // r * cos(u)
        const float z = radius * sinf(stackAngle); // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for (uint32_t j = 0; j <= sectorCount; ++j) {
            const float sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            const float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            const float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            const float nx = x * lengthInv;
            const float ny = y * lengthInv;
            const float nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            const float s = (float)j / sectorCount;
            const float t = (float)i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }

    std::vector<uint32_t> indices;

    for (uint32_t i = 0; i < stackCount; ++i) {
        uint32_t k1 = i * (sectorCount + 1); // beginning of current stack
        uint32_t k2 = k1 + sectorCount + 1; // beginning of next stack

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