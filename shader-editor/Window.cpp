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

    std::array<float, 24> vertices = {
        0.0f, 1.0f, 0.0f, 0.0f, // top-left
        0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        1.0f, 0.0f, 1.0f, 1.0f, // bottom-right

        0.0f, 1.0f, 0.0f, 0.0f, // top-left
        1.0f, 0.0f, 1.0f, 1.0f, // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f // top-right
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

    mQuad.load();

    //

    compileShader(R"(
#version 460 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D finalTexture;

void main()
{
    vec4 color = texture(finalTexture, uv);
//     color.r = 0.0f;
    fragColor = color;
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

        availableSize = ImGui::GetContentRegionAvail();
        float length = std::min(availableSize.x, availableSize.y);

        if (mShaderId != 0) {
            ImGui::Image(mTestTexture.getID(), ImVec2(availableSize), ImVec2(0, 1), ImVec2(1, 0));
            // ImGui::Image(mTexture.getID(), ImVec2(availableSize), ImVec2(1, 1), ImVec2(0, 0));
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
        glViewport(0, 0, mTextureWidth, mTextureHeight);
        glClearColor(0.5f, 0.4f, 0.4f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUseProgram(mShaderId);

        // float xPos { 0.0f };
        // float yPos { 0.0f };
        // float w { availableSize.x  };
        // float h { availableSize.y  };

        // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0.0f));
        // model = glm::scale(model, glm::vec3(w, h, 1.0f));

        // auto orthoProj = glm::ortho(
        //     0.0f,
        //     static_cast<float>(mTextureWidth),
        //     0.0f,
        //     static_cast<float>(mTextureHeight));

        // auto mvp = orthoProj * model;

        // std::println("{} {}", mTextureWidth, mTextureHeight);
        // std::println("{} {}", w, h);

        // int loc = glGetUniformLocation(mShaderId, "proj");
        // glUniformMatrix4fv(
        //     loc,
        //     1,
        //     GL_FALSE,
        //     glm::value_ptr(mvp));

        // // bind a test texture
        // mTestTexture.bind(0);

        // mVao.draw(GL_TRIANGLES);

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
