#include "Cuboid.hpp"

#include <array>
#include <cstdint>

spry::Cuboid::Cuboid()
{
    // std::array<float, (3 + 3 + 2) * 4 * 6> vertices = {
    //     // Format: (x, y, z), (nx, ny, nz), (u, v)
    //     -1, -1, -1, 0, 0, -1, 0, 0, // Back face, bottom-left
    //     1, -1, -1, 0, 0, -1, 1, 0, // Back face, bottom-right
    //     1, 1, -1, 0, 0, -1, 1, 1, // Back face, top-right
    //     -1, 1, -1, 0, 0, -1, 0, 1, // Back face, top-left

    //     -1, -1, 1, 0, 0, 1, 0, 0, // Front face, bottom-left
    //     1, -1, 1, 0, 0, 1, 1, 0, // Front face, bottom-right
    //     1, 1, 1, 0, 0, 1, 1, 1, // Front face, top-right
    //     -1, 1, 1, 0, 0, 1, 0, 1, // Front face, top-left

    //     -1, -1, -1, 0, -1, 0, 0, 0, // Bottom face, bottom-left
    //     1, -1, -1, 0, -1, 0, 1, 0, // Bottom face, bottom-right
    //     1, -1, 1, 0, -1, 0, 1, 1, // Bottom face, top-right
    //     -1, -1, 1, 0, -1, 0, 0, 1, // Bottom face, top-left

    //     -1, 1, -1, 0, 1, 0, 0, 0, // Top face, bottom-left
    //     1, 1, -1, 0, 1, 0, 1, 0, // Top face, bottom-right
    //     1, 1, 1, 0, 1, 0, 1, 1, // Top face, top-right
    //     -1, 1, 1, 0, 1, 0, 0, 1, // Top face, top-left

    //     -1, -1, -1, -1, 0, 0, 0, 0, // Left face, bottom-left
    //     -1, 1, -1, -1, 0, 0, 1, 0, // Left face, bottom-right
    //     -1, 1, 1, -1, 0, 0, 1, 1, // Left face, top-right
    //     -1, -1, 1, -1, 0, 0, 0, 1, // Left face, top-left

    //     1, -1, -1, 1, 0, 0, 0, 0, // Right face, bottom-left
    //     1, 1, -1, 1, 0, 0, 1, 0, // Right face, bottom-right
    //     1, 1, 1, 1, 0, 0, 1, 1, // Right face, top-right
    //     1, -1, 1, 1, 0, 0, 0, 1 // Right face, top-left
    // };

    // std::array<uint32_t, 6 * 6> indices = {
    //     0, 1, 2, 2, 3, 0, // Back face
    //     4, 5, 6, 6, 7, 4, // Front face
    //     8, 9, 10, 10, 11, 8, // Bottom face
    //     12, 13, 14, 14, 15, 12, // Top face
    //     16, 17, 18, 18, 19, 16, // Left face
    //     20, 21, 22, 22, 23, 20 // Right face
    // };

    std::array<float, (3 + 3 + 2) * 4 * 6> vertices {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Top-left

        // Back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // Top-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // Top-right

        // Left face
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-right
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-left
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-left
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-right

        // Right face
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-left

        // Top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Top-left
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Bottom-right
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Bottom-left

        // Bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // Top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
    };

    std::array<uint32_t, 6 * 6> indices {
        0, 1, 2, 2, 3, 0, // correct
        // Back face
        6, 5, 4, 4, 7, 6, // correct
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        14, 13, 12, 12, 15, 14, // correct
        // Top face
        18, 17, 16, 16, 19, 18, // correct
        // Bottom face
        20, 21, 22, 22, 23, 20 // correct?
    };

    // std::array<float, (3 + 3 + 2) * 6 * 6> vertices {
    //     // positions          // normals           // texture coords
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    //     0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    //     0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    //     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    //     -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    //     -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    //     -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    //     0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    //     0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    //     0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    //     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    // };

    std::array<uint32_t, 3> format { 3, 3, 2 };

    mVAO.load(vertices, indices, format, GL_STATIC_DRAW);
    // mVAO.load(vertices, format, 36, GL_STATIC_DRAW);
}

void spry::Cuboid::draw() const
{
    mVAO.draw();
}