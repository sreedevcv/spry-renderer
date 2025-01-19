#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Drawable.hpp"

namespace spry {

struct Transform {
    glm::vec3 mPosition { glm::vec3(0.0f) };
    glm::vec3 mScale { glm::vec3(1.0f) };
    glm::quat mRotation { glm::quat(1.0f, 0.0f, 0.0f, 0.0f) };

    inline glm::mat4 getModel() const
    {
        auto model = glm::translate(glm::mat4(1.0f), mPosition);
        model = model * glm::mat4_cast(mRotation);
        model = glm::scale(model, mScale);
        return model;
    }
};

struct Entity : public Transform {
public:
    Entity(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
    Entity(glm::vec3 position, glm::vec3 scale, glm::quat rotation);

    void update(float delta);

    Drawable* drawable;
    glm::vec3 mVelocity { glm::vec3(0.0f) };
    glm::vec3 mAccleration { glm::vec3(0.0f) };
};

}