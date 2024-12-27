#include "Sphere.hpp"

#include "IndexedVAO.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/trigonometric.hpp"

#include <array>
#include <cstdint>
#include <vector>

void spry::Sphere::load(uint32_t width, uint32_t height)
{
    std::vector<float> vertices;
    vertices.reserve((height + 1) * (width + 1) * (3 + 3 + 2));
    const float PI = glm::pi<float>();

    for (uint32_t i = 0; i < height + 1; i++) {
        const float theta = float(i) * PI / float(height);

        for (uint32_t j = 0; j < width + 1; j++) {
            // Vertices
            const float phi = 2.0f * PI * float(j) / float(width);
            const float x = glm::cos(phi) * glm::sin(theta);
            const float y = glm::cos(theta);
            const float z = glm::sin(phi) * glm::sin(theta);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Normals
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Tex coords
            const float u = 1 - (float(j) / width);
            const float v = 1 - (float(i) / height);
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    std::vector<uint32_t> indices;
    indices.reserve(height * width * 6);

    for (int i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            const uint32_t one = (i * (width + 1)) + j;
            const uint32_t two = one + width + 1;

            indices.push_back(one);
            indices.push_back(two);
            indices.push_back(one + 1);

            indices.push_back(two);
            indices.push_back(two + 1);
            indices.push_back(one + 1);
        }
    }

    std::array<uint32_t, 3> format = { 3, 3, 2 };

    mVao.load(vertices, indices, format, GL_STATIC_DRAW);
}

void spry::Sphere::draw() const
{
    mVao.draw(GL_TRIANGLES);
}