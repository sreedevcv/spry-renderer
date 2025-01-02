#include "Entity.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

spry::Entity3D::Entity3D(glm::vec3 position, glm::vec3 scale)
{
    this->position = position;
    this->scale = scale;
}

void spry::Entity3D::update(float delta)
{
    mVelocity = delta * mAccleration;
    position = delta * mVelocity;
}