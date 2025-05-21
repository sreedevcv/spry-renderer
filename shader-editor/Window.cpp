#include "Window.hpp"

#include "imgui.h"
#include "spdlog/spdlog.h"
#include <cfloat>
#include <cstdio>

Window::Window()
    : spry::Window(600, 400, "Shader Editor")
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Window::ui(float delta)
{
    // ImGui::Text("Hello World");
    // ImGui::Text("FPS: %f", 1.0 / delta);
    // ImGui::Text("Delta: %.2fms", delta * 1000);

    // ImGui::ShowDemoWindow();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Close", "Ctrl+W")) {
                spdlog::info("Close button pressed");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Left
    static int selected = 0;
    {
        ImGui::BeginChild(
            "left pane",
            ImVec2(150, 0),
            ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

        static char buffer[1000];

        ImVec2 size = ImGui::GetContentRegionAvail();

        ImGui::InputTextMultiline(
            "##ed",
            buffer,
            sizeof(buffer),
            size);
        ImGui::EndChild();
    }
    ImGui::SameLine();

    // Right
    {
        ImGui::BeginGroup();
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        ImGui::Text("MyObject: %d", selected);
        ImGui::Separator();
        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
            if (ImGui::BeginTabItem("Description")) {
                ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Details")) {
                ImGui::Text("ID: 0123456789");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::EndChild();
        if (ImGui::Button("Revert")) { }
        ImGui::SameLine();
        if (ImGui::Button("Save")) { }
        ImGui::EndGroup();
    }
}

Window::~Window()
{
}

void Window::onUpdate(float deltaTime)
{
    glClearColor(0.23f, 0.26f, 0.24f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::onMouseMove(double xPosIn, double yPosIn)
{
}

void Window::onMouseScroll(double xOffset, double yOffset)
{
}

void Window::onScreenSizeChange(int width, int height)
{
}

// From https://github.com/ocornut/imgui/issues/3541#issuecomment-712248014
void Window::onImguiDebugDraw(float delta)
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    bool p_open = false;

    if (ImGui::Begin(
            "Example: Simple layout",
            &p_open,
            ImGuiWindowFlags_MenuBar
                | ImGuiWindowFlags_NoDecoration
                | ImGuiWindowFlags_NoResize)) {
        ui(delta);
    }
    ImGui::End();

    ImGui::PopStyleVar(1);
}
