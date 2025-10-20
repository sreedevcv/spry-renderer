#include "Window.hpp"

#include "Texture.hpp"
#include "imgui.h"
#include "spdlog/spdlog.h"
#include "ShaderCompiler.hpp"

#include <format>
#include <string>

Window::Window()
    : spry::Window(800, 600, "Shader Editor")
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    mTestTexture
        .create()
        .setFilterMode(GL_NEAREST)
        .setWrapMode(GL_CLAMP_TO_EDGE)
        .load("/home/sreedev/Pictures/Screenshots/Screenshot From 2025-05-21 22-04-18.png");

    mTexture
        .create()
        .setFilterMode(GL_NEAREST)
        .setWrapMode(GL_CLAMP_TO_EDGE)
        .load(nullptr, mTextureWidth, mTextureHeight, GL_RGB);

    mRenderTarget.load();
    mRenderTarget.attachTextureColor(mTexture);

    mQuad.load();

    std::string fragCode { R"(#version 460 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D finalTexture;

void main()
{
    vec4 color = texture(finalTexture, uv);
    // color.r = 0.0f;
    // fragColor = vec4(0.1, 0.5, 0.8, 1.0);
    fragColor = color;
}
        
)" };

    m_editor.SetText(fragCode);
}

void Window::ui(float delta)
{
    // ImGui::ShowDemoWindow();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Close", "Ctrl+W")) {
                spdlog::info("Close button pressed");
            }
            if (ImGui::MenuItem("Render", "Ctrl+R")) {
                spdlog::info("Rendering");
                spdlog::info("{}", m_editor.GetText());
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Left
    {
        ImGui::BeginChild(
            "left pane",
            ImVec2(400, 0),
            ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

        ImVec2 size = ImGui::GetContentRegionAvail();

        m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
        m_editor.Render("CodeEditor", size);

        ImGui::EndChild();
    }
    ImGui::SameLine();

    // Right
    {
        ImGui::BeginGroup();
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

        availableSize = ImGui::GetContentRegionAvail();

        if (mShaderId != 0) {
            ImGui::Image(mTexture.getID(), ImVec2(availableSize), ImVec2(0, 1), ImVec2(1, 0));
        }

        ImGui::EndChild();
        if (ImGui::Button("Revert")) { }
        ImGui::SameLine();
        if (ImGui::Button("Save")) { }
        ImGui::SameLine();
        ImGui::LabelText("Size", "%s", std::format("{} {}", availableSize.x, availableSize.y).c_str());
        ImGui::EndGroup();
    }
}

Window::~Window()
{
}

void Window::compileShader()
{
    const auto& code = m_editor.GetText();
    const auto result = compile(code.c_str());

    if (result) {
        if (mShaderId != 0) {
            glDeleteProgram(mShaderId);
        }

        mShaderId = result.value();
    } else {
        spdlog::error("{}", result.error());
        mShaderId = 0;
    }
}

void Window::handleKeyboardInputs()
{
    ImGuiIO& io = ImGui::GetIO();
    auto shift = io.KeyShift;
    auto ctrl = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl;
    auto alt = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeyAlt;

    if (ctrl && !shift && !alt && ImGui::IsKeyPressed((ImGuiKey_R))) {
        spdlog::info("Compiling...");
        compileShader();
    }
}

void Window::onUpdate(float deltaTime)
{
    handleKeyboardInputs();

    if (mShaderId != 0) {
        mRenderTarget.bind();
        glViewport(0, 0, mTextureWidth, mTextureHeight);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUseProgram(mShaderId);

        mTestTexture.bind(0);
        mQuad.draw();

        mRenderTarget.unbind();
        glUseProgram(0);
    }

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
