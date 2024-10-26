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
    Shader& bind(const char* path, GLenum type);
    Shader& compile();

    void use();
    void setUniformFloat(const char* name, const float value);
    void setUniformMatrix(const char* name, const glm::mat4& value);
    void setUniformVec(const char* name, const glm::vec4&& value);
    void setUniformVec(const char* name, const glm::vec3&& value);
    void setUniformVec(const char* name, const glm::vec4& value);
    void setUniformVec(const char* name, const glm::vec3& value);
    void setUniformInt(const char* name, const int value) const;
    void unload();

private:
    uint32_t mID;
};

}
