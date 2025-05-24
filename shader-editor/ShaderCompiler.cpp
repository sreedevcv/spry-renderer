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

out vec2 uv;

void main()
{
    const vec2 pos[3] = vec2[](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );

    const vec2 tex[3] = vec2[](
        vec2(0.0, 0.0),
        vec2(2.0, 0.0),
        vec2(0.0, 2.0)
    );

    gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
    uv = tex[gl_VertexID];
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