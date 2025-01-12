#pragma once

#include "BlinnPhongRenderer.hpp"
#include "Entity.hpp"
#include "Materials.hpp"
#include "Scene.hpp"

namespace spry {
namespace dbg {

    void viewTransform(Transform& transform, const char* name);
    void viewMaterial(Material& material, const char* name);
    void viewSceneTree(Scene* scene);
    void viewPointLight(BlinnPhongRenderer::PointLight& pointLight);
}
}
