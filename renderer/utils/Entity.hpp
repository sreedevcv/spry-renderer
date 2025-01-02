#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/quaternion.hpp"

namespace spry {

struct Transform {
    glm::vec3 position { glm::vec3(0.0f) };
    glm::vec3 scale { glm::vec3(1.0f) };
    glm::quat rotation { glm::quat(1.0f, 0.0f, 0.0f, 0.0f) };

    inline glm::mat4 getModel() const
    {
        auto model = glm::translate(glm::mat4(1.0f), position);
        model = glm::scale(model, scale);
        model = model * glm::mat4_cast(rotation);
        return model;
    }
};

class Entity3D : public Transform {
public:
    Entity3D(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

    void update(float delta);

    glm::vec3 mVelocity { glm::vec3(0.0f) };
    glm::vec3 mAccleration { glm::vec3(0.0f) };
};

}