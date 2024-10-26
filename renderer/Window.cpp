#include "Window.hpp"

#include <print>

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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (debug_mode) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }
    m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);

    if (m_window == nullptr) {
        std::println("Failed to load window of size {} {}", width, height);
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
        std::println("Failed to initialize GLAD");
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

    std::println("Window and OpenGL(4.6) inititalized");
}

spry::Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
    std::println("Window destroyed");
}

void spry::Window::start()
{
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(m_window)) {
        double currTime = glfwGetTime();
        float deltaTime = static_cast<float>(currTime - prevTime);
        prevTime = currTime;

        onUpdate(deltaTime);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

bool spry::Window::isKeyPressed(int key)
{
    if (glfwGetKey(m_window, key) == GLFW_PRESS) {
        return true;
    }
    return false;
}

bool spry::Window::isKeyReleased(int key)
{
    if (glfwGetKey(m_window, key) == GLFW_RELEASE) {
        return true;
    }
    return false;
}

void spry::Window::setMouseCapture(bool capture)
{
    if (capture) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

double spry::Window::getGlobalTime()
{
    return glfwGetTime();
}

void spry::Window::setWireFrameMode(bool value)
{
    if (value) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void spry::Window::closeWindow()
{
    glfwSetWindowShouldClose(m_window, true);
}

GLFWwindow* spry::Window::get_window()
{
    return m_window;
}

void spry::Window::onMouseMove(double x_pos_in, double y_pos_in)
{
}

void spry::Window::onMouseScroll(double x_offset, double y_offset)
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

    std::println("`````````````````````````````````````````````````````");
    std::println("Debug message ({}): {}", id, message);

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        std::print("Source: API");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::print("Source: Window System");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::print("Source: Shader Compiler");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::print("Source: Third Party");
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::print("Source: Application");
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::print("Source: Other");
        break;
    }
    std::println();

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::print("Type: Error");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::print("Type: Deprecated Behaviour");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::print("Type: Undefined Behaviour");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::print("Type: Portability");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::print("Type: Performance");
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::print("Type: Marker");
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::print("Type: Push Group");
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::print("Type: Pop Group");
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::print("Type: Other");
        break;
    }
    std::println();

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        std::print("Severity: high");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::print("Severity: medium");
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::print("Severity: low");
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::print("Severity: notification");
        break;
    }
    std::println("\n");
}
