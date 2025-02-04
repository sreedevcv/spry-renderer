#include <cstdlib>
#include <print>
#include <utility>

#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DefaultAxes.hpp"
#include "Entity.hpp"
#include "FullScreenQuad.hpp"
#include "GLFW/glfw3.h"
#include "HeightMapGenerator.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "TerrainMeshGenerator.hpp"
#include "Texture.hpp"
#include "TextureRenderTarget.hpp"
#include "Toggle.hpp"
#include "Window.hpp"

#include "glm/ext/vector_float3.hpp"
#include "imgui.h"

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : spry::Window(width, height, "Test", true)
        , camera(width, height)
    {
        setMouseCapture(true);
        camera.setPosition(glm::vec3(40.0f, 60.0f, 0.0f));
        spry::setCulling(false);
        spry::setDepthTest(true);
        glCullFace(GL_BACK);
        glViewport(0, 0, width, height);

        sphere.load(30, 30);
        plane.load(30, 30);
        scene.load(&camera);

        spry::setWireFrameMode(false);

        map.load(mapWidth, mapHeight, mapRes);
        mesh.load(map.getHeightMap().mWidth, map.getHeightMap().mHeight);

        targetTexture
            .create()
            .setFilterMode(GL_LINEAR)
            .setWrapMode(GL_CLAMP_TO_BORDER)
            .load(nullptr, width, height, GL_RGBA);

        quad.laod();
        target.load();
        target.attachTextureColor(targetTexture);
    }

private:
    spry::Camera camera;
    spry::Sphere sphere;
    spry::Cuboid cube;
    spry::Plane plane;
    spry::DefaultAxes scene;
    float updateTime;
    float prevTime;
    spry::TextureRenderTarget target;
    spry::Texture targetTexture;
    spry::FullScreenQuad quad;
    spry::TerrainMeshGenerator mesh;
    spry::HeightMapGenerator map;

    void onUpdate(float deltaTime) override
    {
        float time = glfwGetTime();
        processInput(deltaTime);

        // target.bind();
        {
            glClearColor(0.2f, 0.2f, 0.24f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene.draw();

            mesh.draw(map.getHeightMap(), camera);
        }
        // target.unbind();

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // map.getHeightMap().bind(0);
        // quad.draw();

        // closeWindow();

        updateTime = time - prevTime;
        prevTime = time;
    }

    spry::Toggle toggle;
    spry::Toggle debugToggle;

    void processInput(float deltaTime)
    {
        toggle.update(deltaTime);
        debugToggle.update(deltaTime);

        if (isKeyPressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
        if (isKeyPressed(GLFW_KEY_ENTER) && debugToggle.canToggle()) {
            setMouseCapture(debugToggle.toggle());
        }

        camera.processInputDefault(*this, deltaTime);
    }

    void onMouseMove(double xPosIn, double yPosIn) override
    {
        if (debugToggle.getValue()) {
            camera.onMouseMoveDefault(xPosIn, yPosIn);
        }
    }

    void onMouseScroll(double xOffset, double yOffset) override
    {
        if (debugToggle.getValue()) {
            camera.onMouseScrollDefault(yOffset);
        }
    }

    void onScreenSizeChange(int width, int height) override
    {
        camera.setScreenSize(width, height);
    }

    int32_t mapWidth = 1000, mapHeight = 1000;
    float mapRes = 0.1;
    void onImguiDebugDraw(float delta) override
    {
        ImGui::Text("Update Time: %.2f", updateTime);
        ImGui::Text("FPS: %f", 1.0 / delta);
        ImGui::Text("Delta: %.2fms", delta * 1000);
        ImGui::Separator();
        ImGui::SliderInt("MapWidth", &mapWidth, 1, 5000);
        ImGui::SliderInt("MapHeight", &mapHeight, 1, 5000);
        ImGui::SliderFloat("MapRes", &mapRes, 0.0f, 10.0f);

        if (ImGui::Button("Generate Height Map")) {
            map.load(mapWidth, mapHeight, mapRes);
        }

        const auto& texture = map.getHeightMap();
        const auto targetWidth = ImGui::GetColumnWidth() - 20.0f;
        const auto targetHeight = texture.mHeight / float(texture.mWidth) * targetWidth;
        ImGui::Image(texture.getID(), ImVec2 { targetWidth, targetHeight });
    }
};

int main()
{
    TestWindow test(1000, 900);
    test.start();

    return 0;
}
