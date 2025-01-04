#pragma once

#include "Line.hpp"
#include "Camera.hpp"

#include "Shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include <cstdint>

namespace spry {

class DefaultScene {
public:
    DefaultScene();
    ~DefaultScene() = default;

    void load(const Camera* camera, uint32_t length = 1000.0f);
    void process(float delta);
    void draw() const;

private:
    spry::Line xAxis;
    spry::Line yAxis;
    spry::Line zAxis;
    glm::mat4 mModel;
    const spry::Shader& mShader;
    const Camera* mCamera;
};

}