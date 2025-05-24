
#include "Camera.hpp"
#include "Cuboid.hpp"
#include "DefaultAxes.hpp"
#include "FastNoiseLite.h"
#include "FullScreenQuad.hpp"
#include "HeightMapGenerator.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "TerrainMeshGenerator.hpp"
#include "Texture.hpp"
#include "TextureRenderTarget.hpp"
#include "Toggle.hpp"
#include "Window.hpp"

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
        mesh.load(map.mWidth, map.mHeight);
        mHeightMap = spry::HeightMapGenerator::createTextureFromHeightMap(
            map.mBuffer,
            map.mWidth,
            map.mHeight);

        targetTexture
            .create()
            .setFilterMode(GL_LINEAR)
            .setWrapMode(GL_CLAMP_TO_BORDER)
            .load(nullptr, width, height, GL_RGBA);

        quad.load();
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
    spry::Texture mHeightMap;

    void onUpdate(float deltaTime) override
    {
        float time = glfwGetTime();
        processInput(deltaTime);

        target.bind();
        {
            glClearColor(0.2f, 0.2f, 0.24f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene.draw();

            mesh.draw(mHeightMap, camera);
        }
        target.unbind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        targetTexture.bind(0);
        quad.draw();

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
    float frequency = 0.01;
    int fractalOctaves = 3;
    float fractalLacunarity = 2.0;
    int currentEnum = 0;
    float fractalGain = 0.5;

    static constexpr const char* fractalTypeMap[] = {
        [FastNoiseLite::FractalType_None] = "FractalType_None",
        [FastNoiseLite::FractalType_FBm] = "FractalType_FBm",
        [FastNoiseLite::FractalType_Ridged] = "FractalType_Ridged",
        [FastNoiseLite::FractalType_PingPong] = "FractalType_PingPong",
        [FastNoiseLite::FractalType_DomainWarpProgressive] = "FractalType_DomainWarpProgressive",
        [FastNoiseLite::FractalType_DomainWarpIndependent] = "FractalType_DomainWarpIndependent",
    };

    void onImguiDebugDraw(float delta) override
    {
        ImGui::Text("Update Time: %.2f", updateTime);
        ImGui::Text("FPS: %f", 1.0 / delta);
        ImGui::Text("Delta: %.2fms", delta * 1000);

        ImGui::Separator();
        ImGui::SliderInt("MapWidth", &mapWidth, 1, 5000);
        ImGui::SliderInt("MapHeight", &mapHeight, 1, 5000);

        ImGui::Separator();
        ImGui::SliderFloat("freq", &frequency, 0.0f, 20.0f);
        ImGui::SliderInt("frac-octaves", &fractalOctaves, 1, 20);
        ImGui::SliderFloat("frac-lacunarity", &fractalLacunarity, 0.0f, 20.0f);
        ImGui::SliderFloat("frac-gain", &fractalGain, 0.0f, 20.0f);
        ImGui::ListBox(
            "Fractal-types",
            &currentEnum,
            fractalTypeMap,
            sizeof(fractalTypeMap));

        if (ImGui::Button("Generate Height Map")) {
            map.mFastNoiseLite.SetFrequency(frequency);
            map.mFastNoiseLite.SetFractalOctaves(fractalOctaves);
            map.mFastNoiseLite.SetFractalLacunarity(fractalLacunarity);
            map.mFastNoiseLite.SetFractalType(static_cast<FastNoiseLite::FractalType>(currentEnum));

            map.load(mapWidth, mapHeight, mapRes);

            mHeightMap = spry::HeightMapGenerator::createTextureFromHeightMap(
                map.getHeightMap(),
                map.mWidth,
                map.mHeight);
        }

        const auto targetWidth = ImGui::GetColumnWidth() - 20.0f;
        const auto targetHeight = mHeightMap.mHeight / float(mHeightMap.mWidth) * targetWidth;
        ImGui::Image(
            mHeightMap.getID(),
            ImVec2 {
                targetWidth,
                targetHeight });
    }
};

int main()
{
    TestWindow test(1000, 900);
    test.start();

    return 0;
}
