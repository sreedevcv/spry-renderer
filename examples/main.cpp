
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
#include <format>

class TestWindow : public spry::Window {
public:
    TestWindow(int width, int height)
        : spry::Window(width, height, "Test", true)
        , camera(width, height)
    {
        setMouseCapture(true);
        camera.setPosition(glm::vec3(-1231.0f, 388.0f, 804.0f));
        spry::setCulling(false);
        spry::setDepthTest(true);
        glCullFace(GL_BACK);
        glViewport(0, 0, width, height);

        sphere.load(30, 30);
        plane.load(30, 30);
        scene.load(&camera);

        spry::setWireFrameMode(false);

        map.generate(mapWidth, mapHeight);
        mesh.load(map.mWidth, map.mHeight);
        mHeightMap = map.createTextureFromHeightMap(
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

        float freq = 1.0f;
        float ampl = 1.0f;
        for (auto& config : configs) {
            config.amplitude = ampl;
            config.frequency = freq;
            ampl /= 2;
            freq *= 2;
        }
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
    struct NoiseConfig {
        float frequency = 0.01;
        int fractalOctaves = 3;
        float fractalLacunarity = 2.0;
        float fractalGain = 0.5;
        int currentEnum = FastNoiseLite::FractalType_FBm;
        float amplitude = 1.0f;
    };
    NoiseConfig configs[3];

    static constexpr const char* fractalTypeMap[]
        = {
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
        ImGui::Text("Position: %.2fms %.2fms %.2fms", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);

        ImGui::Separator();
        ImGui::SliderInt("MapWidth", &mapWidth, 1, 5000);
        ImGui::SliderInt("MapHeight", &mapHeight, 1, 5000);
        ImGui::SliderFloat("power", &map.mPower, 0, 5);

        ImGui::Separator();
        for (size_t i = 0; i < map.mGenerators.size(); i++) {
            if (ImGui::CollapsingHeader(std::format("Noise Gen {}", i).c_str())) {
                ImGui::SliderFloat(std::format("freq {}", i).c_str(), &configs[i].frequency, 0.0f, 5.0f);
                ImGui::SliderFloat(std::format("amplitude {}", i).c_str(), &configs[i].amplitude, 0.0f, 5.0f);
                // ImGui::SliderInt(std::format("frac-octaves {}", i).c_str(), &configs[i].fractalOctaves, 1, 20);
                // ImGui::SliderFloat(std::format("frac-lacunarity {}", i).c_str(), &configs[i].fractalLacunarity, 0.0f, 20.0f);
                // ImGui::SliderFloat(std::format("frac-gain {}", i).c_str(), &configs[i].fractalGain, 0.0f, 20.0f);
                // ImGui::ListBox(
                //     std::format("Fractal-types {}", i).c_str(),
                //     &configs[i].currentEnum,
                //     fractalTypeMap,
                //     sizeof(fractalTypeMap) / sizeof(fractalTypeMap[0]));
            }
        }

        if (ImGui::Button("Generate Height Map")) {
            for (size_t i = 0; i < map.mGenerators.size(); i++) {
                map.mGenerators[i].first.SetFrequency(configs[i].frequency);
                // map.mGenerators[i].first.SetFractalOctaves(configs[i].fractalOctaves);
                // map.mGenerators[i].first.SetFractalLacunarity(configs[i].fractalLacunarity);
                // map.mGenerators[i].first.SetFractalType(static_cast<FastNoiseLite::FractalType>(configs[i].currentEnum));
                map.mGenerators[i].second = configs[i].amplitude;
            }

            map.generate(mapWidth, mapHeight);
            mHeightMap = map.createTextureFromHeightMap(map.mWidth, map.mHeight);
        }

        const auto targetWidth = ImGui::GetColumnWidth() - 20.0f;
        const auto targetHeight = mHeightMap.mHeight / float(mHeightMap.mWidth) * targetWidth;
        ImGui::Image(mHeightMap.getID(), ImVec2 { targetWidth, targetHeight });
    }
};

int main()
{
    TestWindow test(1400, 1000);
    test.start();

    return 0;
}
