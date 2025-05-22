#include "ShaderCompiler.hpp"

#include "glad/glad.h"

#include "spdlog/spdlog.h"
#include <expected>

static std::expected<int32_t, std::string> compileSingleshader(const char* shader, GLenum shaderType)
{
    int32_t id = glCreateShader(shaderType);
    glShaderSource(id, 1, &shader, nullptr);
    glCompileShader(id);

    int success;
    char log[2048];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(id, 2048, nullptr, log);
        spdlog::error("Failed to compile shader");
        return std::unexpected { log };
    } else {
        return id;
    }
}

std::expected<int32_t, std::string> compile(const char* source)
{
    const char* vertexShaderSource = R"(
#version 460 core

layout (location = 0) in vec4 vertex;    // vec2 pos, vec2 tex

out vec2 texCoords;

uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(vertex.xy, 0.0, 1.0);
    texCoords = vertex.zw;
}

)";

    const auto vertexShader = compileSingleshader(vertexShaderSource, GL_VERTEX_SHADER);

    if (!vertexShader) {
        return std::unexpected { std::move(vertexShader.error()) };
    }

    const auto fragmentShader = compileSingleshader(source, GL_FRAGMENT_SHADER);

    if (!fragmentShader) {
        return std::unexpected { std::move(fragmentShader.error()) };
    }

    int32_t id = glCreateProgram();

    glAttachShader(id, vertexShader.value());
    glAttachShader(id, fragmentShader.value());
    glLinkProgram(id);

    int success;
    char log[2048];
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(id, 2048, nullptr, log);
        spdlog::error("Failed to link program");
        return std::unexpected {log};
    }

    glDeleteShader(vertexShader.value());
    glDeleteShader(fragmentShader.value());

    spdlog::info("Compiled shader: {}", id);

    return id;
}