#include "Scene.hpp"
#include <utility>

void spry::Scene::load()
{
}

void spry::Scene::process(float delta)
{
    update(delta);

    for (const auto& scene : mChildren) {
        scene->process(delta);
    }
}

void spry::Scene::draw(const glm::mat4& model, const Shader& shader) const
{
    const auto entityModel = model * getModel();

    shader.setUniformMatrix("model", entityModel);
    drawable->draw();

    for (const auto& scene : mChildren) {
        scene->draw(entityModel, shader);
    }
}

void spry::Scene::addChild(std::unique_ptr<Scene> child)
{
    mChildren.push_back(std::move(child));
}

spry::Scene* spry::Scene::getChild(int n) const
{
    return mChildren[n].get();
}

uint32_t spry::Scene::getChildrenSize() const
{
    return mChildren.size();
}