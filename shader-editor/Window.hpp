#include "gl/Window.hpp"

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
};