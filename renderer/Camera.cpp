#include "Camera.hpp"

#include <glm/glm.hpp>

spry::Camera::Camera(int screenWidth, int screenHeight)
    : mScreenWidth { screenWidth }
    , mScreenHeight { screenHeight }
{
    aspectRatio = static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight);
    updateCameraVectors();
}

glm::mat4 spry::Camera::getViewMatrix() const
{
    return glm::lookAt(mPosition, (mPosition + mFront), mUp);
}

glm::mat4 spry::Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(mZoom), aspectRatio, mNearPoint, mFarPoint);
}

void spry::Camera::setScreenSize(const int width, const int height)
{
    mScreenWidth = width;
    mScreenHeight = height;
    aspectRatio = static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight);
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

    if (constrain_pitch) {
        mPitch = glm::clamp(mPitch, -89.0f, 89.0f);
    }

    updateCameraVectors();
}

void spry::Camera::onMouseScrollDefault(float y_offset)
{
    mZoom -= y_offset;

    if (mZoom < 0.1f) {
        mZoom = 0.1f;
    } else if (mZoom > 45.0f) {
        mZoom = 45.0f;
    }
}

void spry::Camera::processInputDefault(const Window& window, float delta)
{
    if (window.isKeyPressed(GLFW_KEY_W)) {
        processMovement(spry::Camera::movement::FORWARD, delta);
    }
    if (window.isKeyPressed(GLFW_KEY_S)) {
        processMovement(spry::Camera::movement::BACKWARD, delta);
    }
    if (window.isKeyPressed(GLFW_KEY_A)) {
        processMovement(spry::Camera::movement::LEFT, delta);
    }
    if (window.isKeyPressed(GLFW_KEY_D)) {
        processMovement(spry::Camera::movement::RIGHT, delta);
    }
    if (window.isKeyPressed(GLFW_KEY_SPACE)) {
        processMovement(spry::Camera::movement::UP, delta);
    }
    if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        processMovement(spry::Camera::movement::DOWN, delta);
    }
}

void spry::Camera::onMouseMoveDefault(double xPosIn, double yPosIn)
{
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (mouseData.firstMouse) {
        mouseData.lastX = xPos;
        mouseData.lastY = yPos;
        mouseData.firstMouse = false;
    }

    float xOffset = xPos - mouseData.lastX;
    float yOffset = mouseData.lastY - yPos;

    mouseData.lastX = xPos;
    mouseData.lastY = yPos;

    processMouseMovement(xOffset, yOffset, true);
}
