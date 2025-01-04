#pragma once

#include "Drawable.hpp"
#include "Entity.hpp"
#include "Shader.hpp"
#include <cstdint>
#include <memory>
#include <vector>

namespace spry {

class Scene : public Entity {
public:
    Scene(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
    Scene(Drawable* drawable, const char* name, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
    virtual ~Scene() = default;

    virtual void load();
    virtual void process(float delta);
    virtual void draw(const glm::mat4& model, const Shader& shader) const;

    Scene* getChild(int n) const;
    uint32_t getChildrenSize() const;
    void addChild(std::unique_ptr<Scene> child);

    const char* mName;

private:
    std::vector<std::unique_ptr<Scene>> mChildren;
};

}