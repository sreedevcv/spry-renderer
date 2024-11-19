#pragma once

#include <cstdint>
#include <glad/glad.h>

#include <glm/mat4x4.hpp>

namespace spry {

class SingleShader {
public:
    unsigned int mID;

    SingleShader(const char* shaderpath, GLenum shaderType);
    ~SingleShader();

private:
    const char* mShaderPath;
};

class Shader {
public:
    Shader();
    ~Shader() = default;

    Shader(Shader&& shader);
    Shader& operator=(Shader&& shader);
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;


    Shader& bind(const char* path, GLenum type);
    void compile();

    void bind() const;
    void unbind() const;
    void setUniformFloat(const char* name, const float value) const;
    void setUniformMatrix(const char* name, const glm::mat4& value) const;
    void setUniformVec(const char* name, const glm::vec4&& value) const;
    void setUniformVec(const char* name, const glm::vec3&& value) const;
    void setUniformVec(const char* name, const glm::vec4& value) const;
    void setUniformVec(const char* name, const glm::vec3& value) const;
    void setUniformInt(const char* name, const int value) const;
    void unload() const;

private:
    uint32_t mID;
};

}
