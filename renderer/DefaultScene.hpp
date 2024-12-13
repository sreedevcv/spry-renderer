#pragma once

#include "Scene.hpp"
#include "Line.hpp"
#include "Camera.hpp"

#include "Shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"

namespace spry {

class DefaultScene : public Scene {
public:
    DefaultScene(const Camera& camera);
    virtual ~DefaultScene() = default;

    void load() override;
    void process(float delta) override;
    void draw() override;
    void unload() override;

private:
    spry::Line xAxis;
    spry::Line yAxis;
    spry::Line zAxis;
    glm::mat4 mModel;
    const spry::Shader& mShader;
    const Camera& mCamera;
};

}