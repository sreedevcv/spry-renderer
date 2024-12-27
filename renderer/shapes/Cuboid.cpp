#include "Cuboid.hpp"

#include <array>
#include <cstdint>

spry::Cuboid::Cuboid()
{
    std::array<float, 24 * 5> vertices = {
        // Front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Top-left

        // Back face
        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, // Top-left
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // Top-right

        // Left face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
        -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Top-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, // Top-left

        // Right face
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // Top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Top-left

        // Top face
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // Top-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, // Top-left

        // Bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, // Top-right
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, // Top-left
    };

    // Define indices for drawing the cuboid using triangles
    std::array<uint32_t, 36> indices = {
        // Front face
        0, 1, 2, 2, 3, 0, //
        // Back face
        4, 7, 6, 6, 5, 4, //
        // Left face
        8, 9, 10, 10, 11, 8, //
        // Right face
        12, 13, 14, 14, 15, 12, //
        // Top face
        16, 17, 18, 18, 19, 16, //
        // Bottom face
        20, 21, 22, 22, 23, 20, //
    };

    std::array<uint32_t, 2> format = { 3, 2 };

    mVAO.load(vertices, indices, format, GL_STATIC_DRAW);
}

void spry::Cuboid::draw() const
{
    mVAO.draw();
}