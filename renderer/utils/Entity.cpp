#include "Entity.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

spry::Entity3D::Entity3D(glm::vec3 position, glm::vec3 scale)
    : mPosition { position }
    , mScale { scale }
{
}

glm::mat4 spry::Entity3D::getModel() const
{
    auto model = glm::scale(glm::mat4(1.0f), mScale);
    model = glm::translate(model, mPosition);
    return model;
}

void spry::Entity3D::update(float delta)
{
    mVelocity = delta * mAccleration;
    mPosition = delta * mPosition;
}