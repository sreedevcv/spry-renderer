#pragma once

#include "Entity.hpp"
#include "Shader.hpp"
#include <cstdint>
#include <memory>
#include <vector>

namespace spry {

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void load();
    virtual void process(float delta);
    virtual void draw(const glm::mat4& model, const Shader& shader) const;

    Scene* getChild(int n) const;
    uint32_t getChildrenSize() const;
    void addChild(std::unique_ptr<Scene> child);
    Entity mEntity;

private:
    std::vector<std::unique_ptr<Scene>> mChildren;
};

}