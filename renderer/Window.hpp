#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace spry {

class Window {
public:
    Window(int width, int height, const char* title, bool debug_mode = false);
    virtual ~Window();

    void start();
    void closeWindow() const;

    bool isKeyPressed(int key) const;
    bool isKeyReleased(int key) const;

protected:
    GLFWwindow* get_window() const;

    void setMouseCapture(bool capture) const;

    virtual void onUpdate(float deltaTime) = 0;
    virtual void onMouseMove(double xPosIn, double yPosIn);
    virtual void onMouseScroll(double xOffset, double yOffset);
    virtual void onScreenSizeChange(int width, int height);
    virtual void onImguiDebugDraw(float delta);

private:
    int mWidth;
    int mHeight;

    GLFWwindow* mWindow;
};

void setWireFrameMode(bool value);
void setCulling(bool value);
void setDepthTest(bool value);
void setBlending(bool value);
double getGlobalTime();

}
