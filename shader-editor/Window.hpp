#include "FullScreenQuad.hpp"
#include "Texture.hpp"
#include "gl/Window.hpp"

#include "TextEditor.hpp"
#include "gl/VAO.hpp"
#include "gl/TextureRenderTarget.hpp"
#include "imgui.h"
#include <cstdint>

class Window : public spry::Window {
public:
    Window();
    virtual ~Window();

private:
    virtual void onUpdate(float deltaTime) override;
    virtual void onMouseMove(double xPosIn, double yPosIn) override;
    virtual void onMouseScroll(double xOffset, double yOffset) override;
    virtual void onScreenSizeChange(int width, int height) override;
    virtual void onImguiDebugDraw(float delta) override;

    void ui(float delta);
    void compileShader(const char* source);

    int32_t mShaderId { 0 };
    TextEditor m_editor;
    spry::VAO mVao;
    spry::TextureRenderTarget mRenderTarget;
    spry::Texture mTexture;
    spry::Texture mTestTexture;
    int32_t mTextureWidth {600};
    int32_t mTextureHeight {600};
    ImVec2 availableSize;
    spry::FullScreenQuad mQuad;
};