#pragma once

#include <cstdint>
#include <glad/glad.h>

#include <glm/mat4x4.hpp>

namespace spry {
class Shader {
public:
    Shader();
    ~Shader() = default;

    Shader(Shader&& shader);
    Shader& operator=(Shader&& shader);
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;

    Shader& add(const char* path, GLenum type);
    void compile();

    void bind() const;
    void unbind() const;
    void setUniformBlockBinding(const char* uniformBlockName, uint32_t bindingPoint) const;
    void setUniformFloat(const char* name, const float value) const;
    void setUniformMatrix(const char* name, const glm::mat4& value) const;
    void setUniformVec(const char* name, const glm::vec4&& value) const;
    void setUniformVec(const char* name, const glm::vec3&& value) const;
    void setUniformVec(const char* name, const glm::vec2&& value) const;
    void setUniformVec(const char* name, const glm::ivec2&& value) const;
    void setUniformVec(const char* name, const glm::vec4& value) const;
    void setUniformVec(const char* name, const glm::vec3& value) const;
    void setUniformVec(const char* name, const glm::vec2& value) const;
    void setUniformVec(const char* name, const glm::ivec2& value) const;
    void setUniformInt(const char* name, const int value) const;

private:
    uint32_t mID { 0 };

    void unload() const;
};

}
