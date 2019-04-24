#include "window/window.h"

#include <GLFW/glfw3.h>

void centerWindow(GLFWmonitor *monitor, const GLFWvidmode* mode, GLFWwindow* window) {
	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int xpos = monitorX + (mode->width - windowWidth) / 2;
    int ypos = monitorY + (mode->height - windowHeight) / 2;
	glfwSetWindowPos(window, xpos, ypos);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    static_cast<Window*>(glfwGetWindowUserPointer(window))->OnFramebufferSizeChanged(width, height);
}

void cursorPosCallback(GLFWwindow* window, double posX, double posY) {
    static_cast<Window*>(glfwGetWindowUserPointer(window))->OnCursorPositionChanged(posX, posY);
}

InputHandler::Executor::Executor(GLFWwindow* handle)
    : m_handle(handle) {

}

void InputHandler::Executor::Close() const {
    glfwSetWindowShouldClose(m_handle, GL_TRUE);
}

bool InputHandler::Executor::IsPressed(Key value) const {
    return glfwGetKey(m_handle, static_cast<int>(value)) == GLFW_PRESS;
}

Window::Window(uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height) {

}

Window::~Window() {
    if (m_window != nullptr) {
        glfwSetCursorPosCallback(m_window, nullptr);
        glfwSetFramebufferSizeCallback(m_window, nullptr);
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool Window::Init(bool fullscreen, std::string& error) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor == nullptr) {
        error = "Failed to get primary monitor";
        return false;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (mode == nullptr) {
        error = "Failed to get monitor mode";
        return false;
    }

    GLFWmonitor* windowMonitor = nullptr;
    if (fullscreen) {
        windowMonitor = monitor;
        m_width = static_cast<uint32_t>(mode->width);
        m_height = static_cast<uint32_t>(mode->height);
    }

    m_window = glfwCreateWindow(static_cast<int>(m_width), static_cast<int>(m_height), "RTGE", windowMonitor, nullptr);
    if (m_window == nullptr) {
        error = "Failed to create window";
        return false;
    }
    glfwSetWindowUserPointer(m_window, this);

    if (!fullscreen) {
        centerWindow(monitor, mode, m_window);
    }

    glfwMakeContextCurrent(m_window);

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    OnFramebufferSizeChanged(width, height);

    glfwGetCursorPos(m_window, &m_cursorX, &m_cursorY);

    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);

    glfwSwapInterval(1);

    return true;
}

void Window::SetInputHandler(std::weak_ptr<InputHandler> handler) {
    if (!handler.lock()) {
        return;
    }
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    m_inputHandler = handler;

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    OnFramebufferSizeChanged(width, height);
}

bool Window::StartFrame() {
    if (glfwWindowShouldClose(m_window)) {
        return false;
    }

    return true;
}

void Window::EndFrame() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    if (auto handler = m_inputHandler.lock(); handler) {
        handler->KeyHandler(InputHandler::Executor(m_window));
    }
}

void Window::OnFramebufferSizeChanged(int width, int height) {
    glViewport(0, 0, width, height);
    if (auto handler = m_inputHandler.lock(); handler) {
        handler->ScreenHandler(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    }
}

void Window::OnCursorPositionChanged(double posX, double posY) {
    if (auto handler = m_inputHandler.lock(); handler) {
        handler->MouseHandler(float(m_cursorX - posX), float(m_cursorY - posY));
    }
    m_cursorX = posX;
    m_cursorY = posY;
}