#include "Entity.hpp"

spry::Entity::Entity(glm::vec3 position, glm::vec3 scale)
{
    this->mPosition = position;
    this->mScale = scale;
}

spry::Entity::Entity(glm::vec3 position, glm::vec3 scale, glm::quat rotation)
{
    this->mPosition = position;
    this->mScale = scale;
    this->mRotation = rotation;
}

void spry::Entity::update(float delta)
{
    mVelocity += delta * mAccleration;
    mPosition += delta * mVelocity;
}