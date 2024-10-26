#include "Shader.hpp"

#include <fstream>
#include <print>
#include <sstream>
#include <string>

#include <glm/gtc/type_ptr.hpp>

spry::SingleShader::SingleShader(const char* shaderpath, GLenum shaderType)
    : mShaderPath { shaderpath }
{
    std::ifstream shaderFile;
    shaderFile.open(mShaderPath);
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
        std::println("[Error] In shader {} :: {}", shaderpath, log);
    }
}

spry::SingleShader::~SingleShader()
{
    // glDeleteShader(mID);
}

spry::Shader::Shader()
{
    mID = glCreateProgram();
}

spry::Shader::~Shader()
{
    glDeleteProgram(mID);
}

spry::Shader& spry::Shader::bind(const char* path, GLenum type)
{
    SingleShader shader(path, type);
    glAttachShader(mID, shader.mID);
    // sids.
    return *this;
}

spry::Shader& spry::Shader::compile()
{
    glLinkProgram(mID);

    int success;
    char log[1024];
    glGetProgramiv(mID, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(mID, 1024, nullptr, log);
        std::println("[Error] In Linking :: {}", log);
    }

    return *this;
}

void spry::Shader::use()
{
    glUseProgram(mID);
}

void spry::Shader::set_uniform_float(const char* name, const float value)
{
    int loc = glGetUniformLocation(mID, name);
    glUniform1f(loc, value);
}

void spry::Shader::set_uniform_matrix(const char* name, const glm::mat4& value)
{
    int loc = glGetUniformLocation(mID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void spry::Shader::set_uniform_vec(const char* name, const glm::vec3&& value)
{
    int loc = glGetUniformLocation(mID, name);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::set_uniform_vec(const char* name, const glm::vec4&& value)
{
    int loc = glGetUniformLocation(mID, name);
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::set_uniform_vec(const char* name, const glm::vec3& value)
{
    int loc = glGetUniformLocation(mID, name);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::set_uniform_int(const char* name, const int value) const
{
    int loc = glGetUniformLocation(mID, name);
    glUniform1i(loc, value);
}

void spry::Shader::set_uniform_vec(const char* name, const glm::vec4& value)
{
    int loc = glGetUniformLocation(mID, name);
    glUniform4fv(loc, 1, glm::value_ptr(value));
}
