#include "DefaultScene.hpp"

spry::DefaultScene::DefaultScene(Camera& camera)
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
        .bind(RES_PATH "shaders/Line.vert.glsl", GL_VERTEX_SHADER)
        .bind(RES_PATH "shaders/Line.frag.glsl", GL_FRAGMENT_SHADER)
        .compile();
}

void spry::DefaultScene::process(float delta)
{
}

void spry::DefaultScene::update(float delta)
{
    mShader.use();
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
