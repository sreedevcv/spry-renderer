#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

namespace spry {

class Entity3D {
public:
    Entity3D(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

    void setPostion(glm::vec3 position);
    void setScale(glm::vec3 scale);
    const glm::mat4& getModel() const;

protected:
    glm::vec3 mPosition;
    glm::vec3 mScale;
    glm::mat4 mModel;
};

}