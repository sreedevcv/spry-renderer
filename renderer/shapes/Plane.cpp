#include "Plane.hpp"

#include <cstdint>
#include <vector>

void spry::Plane::load(uint32_t width, uint32_t height)
{
    Buffer buffer = getBuffers(width, height);
    mVao.load(buffer.vertices, buffer.indices, buffer.format, GL_STATIC_DRAW);
}

spry::Plane::Buffer spry::Plane::getBuffers(uint32_t width, uint32_t height) const
{
    std::vector<float> vertices((width + 1) * (height + 1) * (3 + 3 + 2));
    uint32_t idx = 0;

    for (float z = 0.0f; z < height + 1; z++) {
        for (float x = 0.0f; x < width + 1; x++) {
            // Actual vertices
            vertices[idx++] = x;
            vertices[idx++] = 0.0f;
            vertices[idx++] = z;
            // Normals (0, 1, 0)
            vertices[idx++] = 0.0f;
            vertices[idx++] = 1.0f;
            vertices[idx++] = 0.0f;
            // Tex coords
            vertices[idx++] = x / (width);
            vertices[idx++] = z / (height);
        }
    }

    std::vector<uint32_t> indices((width * height) * 2 * 3);
    int index = 0;

    for (uint32_t j = 0; j < height; j++) {
        for (uint32_t i = 0; i < width; i++) {
            uint32_t idx = j * (width + 1) + i;
            // First triangle
            indices[index++] = idx;
            indices[index++] = (width + 1) + idx;
            indices[index++] = idx + 1;
            // Second triangle
            indices[index++] = (width + 1) + idx;
            indices[index++] = (width + 1) + idx + 1;
            indices[index++] = idx + 1;
        }
    }

    std::vector<uint32_t> format = { 3, 3, 2 };

    return Buffer {
        .vertices = std::move(vertices),
        .indices = std::move(indices),
        .format = std::move(format),
    };
}

void spry::Plane::draw() const
{
    mVao.draw();
}
