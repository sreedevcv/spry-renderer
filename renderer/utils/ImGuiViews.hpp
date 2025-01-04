#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
namespace spry {
namespace dbg {

    void viewTransform(Transform& transform, const char* name);
    void viewSceneTree(Scene* scene);
}
}
