#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace spry {

class Window {
public:
    Window(int width, int height, const char* title, bool debug_mode = false);
    virtual ~Window();

    bool isKeyPressed(int key);
    bool isKeyReleased(int key);
    double getGlobalTime();
    void setMouseCapture(bool capture);
    void setWireFrameMode(bool value);

    void start();
    void closeWindow();
    GLFWwindow* get_window();

protected:
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onMouseMove(double x_pos_in, double y_pos_in);
    virtual void onMouseScroll(double x_offset, double y_offset);
    virtual void onScreenSizeChange(int width, int height);

private:
    int m_width;
    int m_height;

    GLFWwindow* m_window;
};

}
