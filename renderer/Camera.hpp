#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace spry {

class Camera {
public:
    glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 2.0f);
    glm::vec3 mFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 mRight = glm::vec3(1.0f, 0.0f, 0.0f);

    float mYaw = -90.0f;
    float mPitch = 0.0f;

    // Options
    float mSpeed = 10.0f;
    float mMouseSensitivity = 0.1f;
    float mZoom = 45.0f;
    float mNearPoint = 0.1f;
    float mFarPoint = 1000.0f;

    Camera(int screenWidth, int screenHeight);
    ~Camera() = default;

    enum class movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    struct mouseDataStruct {
        float firstMouse = true;
        float lastX;
        float lastY;
        // bool captured = false;
    } mouseData;

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processMovement(movement m, float delta_time);
    void processMouseMovement(float x_offset, float y_offset, bool constrain_pitch = true);
    void processMouseScroll(float y_offset);

    void setScreenSize(const int width, const int height);
    void setPosition(glm::vec3&& position);

private:
    int mWidth;
    int mHeight;
    float aspectRatio;

    void updateCameraVectors();
};

}