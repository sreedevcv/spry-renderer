#include "DefaultScene.hpp"

spry::DefaultScene::DefaultScene(const Camera& camera)
    : mCamera(camera)
{
}

void spry::DefaultScene::load()
{
    xAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
    yAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
    zAxis.setEndPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

    mModel = glm::mat4(1.0f);

    mShader
        .add(RES_PATH "shaders/Line.vert.glsl", GL_VERTEX_SHADER)
        .add(RES_PATH "shaders/Line.frag.glsl", GL_FRAGMENT_SHADER)
        .compile();
}

void spry::DefaultScene::process(float delta)
{
}

void spry::DefaultScene::draw()
{
    mShader.bind();
    mShader.setUniformMatrix("projection", mCamera.getProjectionMatrix());
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
    mShader.unload();
    xAxis.unload();
    yAxis.unload();
    zAxis.unload();
}
