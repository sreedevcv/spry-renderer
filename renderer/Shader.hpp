#pragma once

#include <cstdint>
#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <vector>


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
    ~Shader();
    Shader& bind(const char* path, GLenum type);
    Shader& compile();

    void use();
    void set_uniform_float(const char* name, const float value);
    void set_uniform_matrix(const char* name, const glm::mat4& value);
    void set_uniform_vec(const char* name, const glm::vec4&& value);
    void set_uniform_vec(const char* name, const glm::vec3&& value);
    void set_uniform_vec(const char* name, const glm::vec4& value);
    void set_uniform_vec(const char* name, const glm::vec3& value);
    void set_uniform_int(const char* name, const int value) const;

private:
    uint32_t mID;
};

}
