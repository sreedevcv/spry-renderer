#include "Window.hpp"

#include "imgui.h"

Window::Window()
    : spry::Window(600, 400, "Shader Editor")
{
}

void Window::ui(float delta)
{
    ImGui::Text("Hello World");
    ImGui::Text("FPS: %f", 1.0 / delta);
    ImGui::Text("Delta: %.2fms", delta * 1000);
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
    ImGui::Begin("temp", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    ui(delta);

    ImGui::End();
    ImGui::PopStyleVar(1);
}
