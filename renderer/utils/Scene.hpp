#pragma once

#include "Drawable.hpp"
#include "Entity.hpp"
#include "Materials.hpp"
#include "Shader.hpp"
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

namespace spry {

class Scene : public Entity {
public:
    Scene(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
    Scene(Drawable* drawable,
        const char* label,
        const char* materialName,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f));

    virtual ~Scene() = default;

    virtual void load();
    virtual void process(float delta);
    virtual void draw(const glm::mat4& model, const Shader& shader) const;

    template <typename Func>
    void draw(const glm::mat4& model, const Shader& shader, Func f) const
    {

        const auto entityModel = model * getModel();
        shader.setUniformMatrix("model", entityModel);
        f(this, shader);
        drawable->draw();

        for (const auto& scene : mChildren) {
            scene->draw(entityModel, shader, f);
        }
    }

    Scene* getChild(int n) const;
    uint32_t getChildrenSize() const;
    void addChild(std::unique_ptr<Scene> child);

    const char* mName { "" };
    std::string_view mMaterialName { "emerald" };
    Material mMaterial;

private:
    std::vector<std::unique_ptr<Scene>> mChildren;
};

}