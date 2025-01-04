#include "ImGuiViews.hpp"
#include "Scene.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "imgui.h"
#include <cstdint>
#include <format>

void spry::dbg::viewTransform(Transform& transform, const char* name)
{
    ImGui::DragFloat3(std::format("{} pos", name).c_str(), (float*)&transform.mPosition, 1.0f, -100.0f, 100.0f);
    ImGui::DragFloat3(std::format("{} scale", name).c_str(), (float*)&transform.mScale, 1.0f, -100.0f, 100.0f);
    if (ImGui::DragFloat4(std::format("{} orien", name).c_str(), (float*)&transform.mRotation, 0.01f, -1.0f, 1.0f)) {
        transform.mRotation = glm::normalize(transform.mRotation);
    }
}

void spry::dbg::viewSceneTree(Scene* scene)
{
    if (ImGui::TreeNode(scene->mName)) {
        viewTransform(*scene, scene->mName);
        ImGui::Separator();
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