#include "Entity.hpp"

spry::Entity::Entity(glm::vec3 position, glm::vec3 scale)
{
    this->mPosition = position;
    this->mScale = scale;
}

void spry::Entity::update(float delta)
{
    mVelocity += delta * mAccleration;
    mPosition += delta * mVelocity;
}