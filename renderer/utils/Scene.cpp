#include "Scene.hpp"
#include "Entity.hpp"
#include "Materials.hpp"
#include <utility>

spry::Scene::Scene(glm::vec3 position, glm::vec3 scale)
    : Entity(position, scale)
{
    mMaterial = *materials.at(mMaterialName);
}

spry::Scene::Scene(
    Drawable* drawable,
    const char* name,
    const char* materialName,
    glm::vec3 position,
    glm::vec3 scale,
    glm::quat rotation)
    : Entity(position, scale, rotation)
    , mName(name)
    , mMaterialName(materialName)
{
    this->drawable = drawable;
    mMaterial = *materials.at(mMaterialName);
}

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