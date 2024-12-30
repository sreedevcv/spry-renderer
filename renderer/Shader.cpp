#include "Shader.hpp"

#include <cstdint>
#include <fstream>
#include <print>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

struct SingleShader {
    unsigned int mID;

    SingleShader(const char* shaderPath, GLenum shaderType)
    {
        std::ifstream shaderFile;
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        std::string shaderCode = shaderStream.str();
        const char* shaderCodeStr = shaderCode.c_str();

        mID = glCreateShader(shaderType);
        glShaderSource(mID, 1, &shaderCodeStr, nullptr);
        glCompileShader(mID);

        int success;
        char log[1024];
        glGetShaderiv(mID, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(mID, 1024, nullptr, log);
            spdlog::error("Failed to compile shader {}:{}", shaderPath, log);
        } else {
            spdlog::info("Compiled shader[{}]: {}", mID, shaderPath);
        }
    }

    ~SingleShader()
    {
        glDeleteShader(mID);
        spdlog::debug("Deleted shader[{}]", mID);
    }
};


spry::Shader::Shader()
{
    mID = glCreateProgram();
}

spry::Shader::Shader(Shader&& shader)
{
    if (this != &shader) {
        mID = std::move(shader.mID);
        shader.mID = 0;
    }
}

spry::Shader& spry::Shader::operator=(Shader&& shader)
{
    if (this != &shader) {
        mID = std::move(shader.mID);
        shader.mID = 0;
    }

    return *this;
}

void spry::Shader::unload() const
{
    glDeleteProgram(mID);
    spdlog::debug("Deleted program[{}]", mID);
}

// Attach a shader to the glProgram
spry::Shader& spry::Shader::add(const char* path, GLenum type)
{
    SingleShader shader(path, type);
    glAttachShader(mID, shader.mID);
    return *this;
}

void spry::Shader::compile()
{
    glLinkProgram(mID);

    int success;
    char log[1024];
    glGetProgramiv(mID, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(mID, 1024, nullptr, log);
        spdlog::error("Failed to link program: {}", log);
    }
}

void spry::Shader::bind() const
{
    glUseProgram(mID);
    spdlog::debug("Using program[{}]", mID);
}

void spry::Shader::unbind() const
{
    glUseProgram(0);
}

void spry::Shader::setUniformBlockBinding(const char* uniformBlockName, uint32_t bindingPoint) const
{
    uint32_t blockIndex = glGetUniformBlockIndex(mID, uniformBlockName);
    glUniformBlockBinding(mID, blockIndex, bindingPoint);
}

void spry::Shader::setUniformFloat(const char* name, const float value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform1f(loc, value);
}

void spry::Shader::setUniformMatrix(const char* name, const glm::mat4& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void spry::Shader::setUniformVec(const char* name, const glm::vec4&& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::setUniformVec(const char* name, const glm::vec3&& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::setUniformVec(const char* name, const glm::vec2&& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform2fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::setUniformVec(const char* name, const glm::ivec2&& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform2iv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::setUniformVec(const char* name, const glm::vec4& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::setUniformVec(const char* name, const glm::vec3& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::setUniformVec(const char* name, const glm::vec2& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform2fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::setUniformVec(const char* name, const glm::ivec2& value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform2iv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::setUniformInt(const char* name, const int value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform1i(loc, value);
}
