#include "DefaultAxes.hpp"

#include "ShaderManager.hpp"

spry::DefaultAxes::DefaultAxes()
    : mShader(ShaderManager::instance().loadAndGet(ShaderManager::LINE))
{
}

void spry::DefaultAxes::load(const Camera* camera, uint32_t length)
{
    xAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
    yAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
    zAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

    mModel = glm::mat4(1.0f);
    mCamera = camera;
}

void spry::DefaultAxes::process(float delta)
{
}

void spry::DefaultAxes::draw() const
{
    mShader.bind();
    mShader.setUniformMatrix("proj", mCamera->getProjectionMatrix());
    mShader.setUniformMatrix("view", mCamera->getViewMatrix());
    mShader.setUniformMatrix("model", mModel);
    mShader.setUniformVec("lineColor", glm::vec3(1.0f, 0.0f, 0.0f));
    xAxis.draw();
    mShader.setUniformVec("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));
    yAxis.draw();
    mShader.setUniformVec("lineColor", glm::vec3(0.0f, 0.0f, 1.0f));
    zAxis.draw();
}
