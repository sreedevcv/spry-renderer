#pragma once

#include "Scene.hpp"
#include "Line.hpp"
#include "Camera.hpp"

#include "Shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"

namespace spry {

class DefaultScene : public Scene {
public:
    DefaultScene();
    virtual ~DefaultScene() = default;

    void load(const Camera* camera);
    void process(float delta) override;
    void draw() const;
    void unload() override;

private:
    spry::Line xAxis;
    spry::Line yAxis;
    spry::Line zAxis;
    glm::mat4 mModel;
    const spry::Shader& mShader;
    const Camera* mCamera;
};

}