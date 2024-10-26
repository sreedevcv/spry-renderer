#include "Camera.hpp"

#include <glm/glm.hpp>

spry::Camera::Camera(int screenWidth, int screenHeight)
    : mWidth { screenWidth }
    , mHeight { screenHeight }
{
    aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
    updateCameraVectors();
}

glm::mat4 spry::Camera::getViewMatrix()
{
    return glm::lookAt(mPosition, (mPosition + mFront), mUp);
}

glm::mat4 spry::Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(mZoom), aspectRatio, mNearPoint, mFarPoint);
}

void spry::Camera::setScreenSize(const int width, const int height)
{
    mWidth = width;
    mHeight = height;
    aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
}

void spry::Camera::setPosition(glm::vec3&& position)
{
    mPosition = position;
}

void spry::Camera::updateCameraVectors()
{
    glm::vec3 updated_front;
    updated_front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    updated_front.y = sin(glm::radians(mPitch));
    updated_front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

    mFront = glm::normalize(updated_front);
}

void spry::Camera::processMovement(movement m, float delta_time)
{
    float velocity = mSpeed * delta_time;

    switch (m) {
    case movement::FORWARD:
        mPosition += mFront * velocity;
        break;

    case movement::BACKWARD:
        mPosition -= mFront * velocity;
        break;

    case movement::LEFT:
        mPosition += glm::normalize(glm::cross(mUp, mFront)) * velocity;
        break;

    case movement::RIGHT:
        mPosition += glm::normalize(glm::cross(mFront, mUp)) * velocity;
        break;

    case movement::UP:
        mPosition.y += velocity;
        break;

    case movement::DOWN:
        mPosition.y -= velocity;
        break;
    }
}

void spry::Camera::processMouseMovement(float x_offset, float y_offset, bool constrain_pitch)
{
    x_offset *= mMouseSensitivity;
    y_offset *= mMouseSensitivity;

    mYaw += x_offset;
    mPitch += y_offset;

    mYaw = std::fmod(mYaw + x_offset, 360.0f);
    // m_yaw = glm::mod( m_yaw + x_offset, 360.0f );

    if (constrain_pitch) {
        mPitch = glm::clamp(mPitch, -89.0f, 89.0f);
    }

    updateCameraVectors();
}

void spry::Camera::processMouseScroll(float y_offset)
{
    mZoom -= y_offset;

    if (mZoom < 0.1f) {
        mZoom = 0.1f;
    } else if (mZoom > 45.0f) {
        mZoom = 45.0f;
    }
}