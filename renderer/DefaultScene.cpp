#include "DefaultScene.hpp"
#include "ShaderManager.hpp"

spry::DefaultScene::DefaultScene(const Camera& camera)
    : mCamera(camera)
    , mShader(ShaderManager::instance().loadAndGet(ShaderManager::LINE))
{
}

void spry::DefaultScene::load()
{
    xAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
    yAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
    zAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

    mModel = glm::mat4(1.0f);
}

void spry::DefaultScene::process(float delta)
{
}

void spry::DefaultScene::draw()
{
    mShader.bind();
    mShader.setUniformMatrix("proj", mCamera.getProjectionMatrix());
    mShader.setUniformMatrix("view", mCamera.getViewMatrix());
    mShader.setUniformMatrix("model", mModel);
    mShader.setUniformVec("lineColor", glm::vec3(1.0f, 0.0f, 0.0f));
    xAxis.draw();
    mShader.setUniformVec("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));
    yAxis.draw();
    mShader.setUniformVec("lineColor", glm::vec3(0.0f, 0.0f, 1.0f));
    zAxis.draw();
}

void spry::DefaultScene::unload()
{
    xAxis.unload();
    yAxis.unload();
    zAxis.unload();
}
