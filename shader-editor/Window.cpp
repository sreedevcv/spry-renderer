#include "Window.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "spdlog/spdlog.h"
#include "ShaderCompiler.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include <algorithm>
#include <format>
#include <print>

Window::Window()
    : spry::Window(800, 600, "Shader Editor")
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    std::array<uint32_t, 1> format { 4 };
    float xPos { 0.0f };
    float yPos { 0.0f };
    float w { static_cast<float>(mTextureWidth) };
    float h { static_cast<float>(mTextureHeight) };

    std::array<float, 24> vertices = {
        xPos, yPos + h, 0.0f, 0.0f, //
        xPos, yPos, 0.0f, 1.0f, //
        xPos + w, yPos, 1.0f, 1.0f, //

        xPos, yPos + h, 0.0f, 0.0f, //
        xPos + w, yPos, 1.0f, 1.0f, //
        xPos + w, yPos + h, 1.0f, 0.0f, //
    };

    mVao.load(
        vertices,
        std::span<uint32_t> { format },
        24,
        GL_STATIC_DRAW);

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

    //

    compileShader(R"(
#version 460 core

in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D text;

void main()
{
    
    // float alpha = texture(text, texCoords).r;
    // vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);
    
    fragColor = texture(text, texCoords).rgba;
}
        
)");
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
    static int selected = 0;
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
        // ImGui::Text("MyObject: %d", selected);
        // ImGui::Separator();
        // if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        //     if (ImGui::BeginTabItem("Description")) {
        //         ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
        //         ImGui::EndTabItem();
        //     }
        //     if (ImGui::BeginTabItem("Details")) {
        //         ImGui::Text("ID: 0123456789");
        //         ImGui::EndTabItem();
        //     }
        //     ImGui::EndTabBar();
        // }

        ImVec2 size = ImGui::GetContentRegionAvail();
        float length = std::min(size.x, size.y);
        mTextureWidth = size.x;
        mTextureHeight = size.y;

        if (mShaderId != 0) {
            ImGui::Image(mTexture.getID(), ImVec2(size));
        }

        ImGui::EndChild();
        if (ImGui::Button("Revert")) { }
        ImGui::SameLine();
        if (ImGui::Button("Save")) { }
        ImGui::SameLine();
        ImGui::LabelText("Size", "%s", std::format("{} {}", size.x, size.y).c_str());
        ImGui::EndGroup();
    }
}

Window::~Window()
{
}

void Window::compileShader(const char* source)
{
    const auto result = compile(source);

    if (result) {
        mShaderId = result.value();
    } else {
        spdlog::error("{}", result.error());
        mShaderId = 0;
    }
}

void Window::onUpdate(float deltaTime)
{
    if (mShaderId != 0) {
        mRenderTarget.bind();

        glClearColor(0.5f, 0.4f, 0.4f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUseProgram(mShaderId);

        auto orthoProj = glm::ortho(
            0.0f,
            static_cast<float>(mTextureWidth),
            0.0f,
            static_cast<float>(mTextureHeight));
        
        std::println("{} {}", mTextureWidth, mTextureHeight);

        int loc = glGetUniformLocation(mShaderId, "proj");
        glUniformMatrix4fv(
            loc,
            1,
            GL_FALSE,
            glm::value_ptr(orthoProj));

        // bind a test texture
        mTestTexture.bind(0);

        mVao.draw(GL_TRIANGLES);

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
