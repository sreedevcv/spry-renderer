#include "ImGuiViews.hpp"

#include "Materials.hpp"
#include "Scene.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "imgui.h"
#include <cstdint>
#include <format>

void spry::dbg::viewTransform(Transform& transform, const char* name)
{
    ImGui::DragFloat3(std::format("{} pos", name).c_str(), &transform.mPosition.x, 1.0f, -100.0f, 100.0f);
    ImGui::DragFloat3(std::format("{} scale", name).c_str(), &transform.mScale.x, 1.0f, -100.0f, 100.0f);
    if (ImGui::DragFloat4(std::format("{} orien", name).c_str(), (float*)&transform.mRotation, 0.01f, -1.0f, 1.0f)) {
        transform.mRotation = glm::normalize(transform.mRotation);
    }
}

void spry::dbg::viewMaterial(Material& material, const char* name)
{
    ImGui::ColorEdit3(std::format("{} ambient", name).c_str(), &material.ambient.r);
    ImGui::ColorEdit3(std::format("{} diffuse", name).c_str(), &material.diffuse.r);
    ImGui::ColorEdit3(std::format("{} specular", name).c_str(), &material.specular.r);
    ImGui::DragFloat(std::format("{} shininess", name).c_str(), &material.shininess, 0.01f, 0.0f, 1.0f);
}

void spry::dbg::viewSceneTree(Scene* scene)
{
    if (ImGui::TreeNode(scene->mName)) {
        if (ImGui::CollapsingHeader("Properties")) {
            viewTransform(*scene, scene->mName);
            ImGui::Separator();
            viewMaterial(scene->mMaterial, scene->mName);
        }
        for (uint32_t i = 0; i < scene->getChildrenSize(); i++) {
            auto child = scene->getChild(i);
            viewSceneTree(child);
        }
        ImGui::TreePop();
    }
    // std::stack<Scene*> stk;
    // stk.push(scene);

    // if (ImGui::TreeNode("Scene Tree")) {
    //     while (!stk.empty()) {
    //         auto* node = stk.top();
    //         stk.pop();

    //         if (ImGui::TreeNode(node->mName)) {
    //             viewTransform(*node, node->mName);
    //             ImGui::TreePop();
    //         }

    //         for (int i = node->getChildrenSize() - 1; i >= 0; i--) {
    //             stk.push(node->getChild(i));
    //         }
    //     }

    //     ImGui::TreePop();
    // }

    // return;
}

void spry::dbg::viewPointLight(BlinnPhongRenderer::PointLight& pointLight)
{
    ImGui::DragFloat3(std::format("pos").c_str(), &pointLight.position.x, 0.5f, -100.0f, 100.0f);
    ImGui::DragFloat(std::format("constant").c_str(), &pointLight.constant, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat(std::format("linear").c_str(), &pointLight.linear, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat(std::format("quadratic").c_str(), &pointLight.quadratic, 0.01f, 0.0f, 1.0f);
    ImGui::ColorEdit3(std::format("ambient").c_str(), &pointLight.ambient.r);
    ImGui::ColorEdit3(std::format("diffuse").c_str(), &pointLight.diffuse.r);
    ImGui::ColorEdit3(std::format("specular").c_str(), &pointLight.specular.r);
}