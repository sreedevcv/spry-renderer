#include "Window.hpp"

#include <print>
#include <spdlog/spdlog.h>

void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);

spry::Window::Window(int width, int height, const char* title, bool debug_mode)
    : m_width(width)
    , m_height(height)
{
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] [T %t] %v");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (debug_mode) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }
    m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);

    if (m_window == nullptr) {
        spdlog::error("Failed to load window of size {} {}", width, height);
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    static const auto mouse_move_callback = [](GLFWwindow* glfw_window, double x_pos_in, double y_pos_in) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window->onMouseMove(x_pos_in, y_pos_in);
    };

    static const auto mouse_scroll_callback = [](GLFWwindow* glfw_window, double x_offset, double y_offset) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window->onMouseScroll(x_offset, y_offset);
    };

    const auto framebuffer_size_callback = [](GLFWwindow* glfw_window, int width, int height) {
        glViewport(0, 0, width, height);
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window->onScreenSizeChange(width, height);
    };

    glfwSetCursorPosCallback(m_window, mouse_move_callback);
    glfwSetScrollCallback(m_window, mouse_scroll_callback);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::error("Failed to initialize GLAD");
        std::exit(-1);
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        // initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    spdlog::info("Window and OpenGL(4.6) inititalized");
}

spry::Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
    spdlog::info("Window destroyed");
}

void spry::Window::start()
{
    spdlog::info("Starting main loop");
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(m_window)) {
        double currTime = glfwGetTime();
        float deltaTime = static_cast<float>(currTime - prevTime);
        prevTime = currTime;

        onUpdate(deltaTime);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    spdlog::info("Exiting main loop");
}

bool spry::Window::isKeyPressed(int key) const
{
    if (glfwGetKey(m_window, key) == GLFW_PRESS) {
        return true;
    }
    return false;
}

bool spry::Window::isKeyReleased(int key) const
{
    if (glfwGetKey(m_window, key) == GLFW_RELEASE) {
        return true;
    }
    return false;
}

void spry::Window::setMouseCapture(bool capture) const
{
    if (capture) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    spdlog::info("Mouse capture is {}", capture);
}

double spry::Window::getGlobalTime() const
{
    return glfwGetTime();
}

void spry::Window::setWireFrameMode(bool value) const
{
    if (value) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    spdlog::info("Wire frame mode is {}", value);
}

void spry::Window::closeWindow() const
{
    glfwSetWindowShouldClose(m_window, true);
}

GLFWwindow* spry::Window::get_window() const
{
    return m_window;
}

void spry::Window::onMouseMove(double xPosIn, double yPosIn)
{
}

void spry::Window::onMouseScroll(double xOffset, double yOffset)
{
}

void spry::Window::onScreenSizeChange(int width, int height)
{
}

void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    const char* srcMsg = "";
    const char* typeMsg = "";
    const char* severityMsg = "";

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        srcMsg = "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        srcMsg = "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        srcMsg = "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        srcMsg = "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        srcMsg = "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        srcMsg = "Source: Other";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        typeMsg = "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        typeMsg = "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        typeMsg = "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        typeMsg = "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        typeMsg = "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        typeMsg = "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        typeMsg = "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        typeMsg = "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        typeMsg = "Type: Other";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        severityMsg = "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severityMsg = "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severityMsg = "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        severityMsg = "Severity: notification";
        break;
    }
    
    spdlog::critical("OpenGL Debug message ({}): {}", id, message);
    spdlog::critical("{}", srcMsg);
    spdlog::critical("{}", typeMsg);
    spdlog::critical("{}", severityMsg);
}
