#include "Entity.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

spry::Entity3D::Entity3D(glm::vec3 position, glm::vec3 scale)
    : mPosition { position }
    , mScale { scale }
{
}

void spry::Entity3D::setPostion(glm::vec3 position)
{
    mPosition = position;
    mModel = glm::scale(glm::mat4(1.0f), mScale);
    mModel = glm::translate(mModel, mPosition);
}

void spry::Entity3D::setScale(glm::vec3 scale)
{
    mScale = scale;
    mModel = glm::scale(glm::mat4(1.0f), mScale);
    mModel = glm::translate(mModel, mPosition);
}

const glm::mat4& spry::Entity3D::getModel() const
{
    return mModel;
}
