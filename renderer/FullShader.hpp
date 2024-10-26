#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

namespace spry {

class ShaderManager;

class FullShader {
private:
    unsigned int ID;
    const char* mVert_shader_path;
    const char* mFrag_shader_path;
    const char* mGeom_shader_path;
    bool mHasCompiled = false;

    void compile_shader_code(const char* vertex, const char* fragment, const char* geometry=nullptr);

public:
    FullShader();
    FullShader(const char* vert_shader_path, const char* fragShaderSource, const char* geom_shader_source=nullptr);
    ~FullShader();

    void set_shader_paths(const char* vert_shader_path, const char* frag_shader_path, const char* geom_shader_path=nullptr);
    void set_shader_code(const char* vert_shader_source, const char* frag_shader_source, const char* geom_shader_source=nullptr);
    void compile();

    void use();
    void set_uniform_float(const char* name, const float value);
    void set_uniform_matrix(const char* name, const glm::mat4& value);
    void set_uniform_vec(const char* name, const glm::vec4&& value);
    void set_uniform_vec(const char* name, const glm::vec3&& value);
    void set_uniform_vec(const char* name, const glm::vec4& value);
    void set_uniform_vec(const char* name, const glm::vec3& value);
    void set_uniform_int(const char* name, const int value) const;

    friend class ShaderManager;
};


class ShaderManager {

public:
    static FullShader simple_shader();
    static FullShader mvp_shader();
    static FullShader create_shader(const char* vert_code, const char* frag_code, const char *geom_code=nullptr);
};
}
