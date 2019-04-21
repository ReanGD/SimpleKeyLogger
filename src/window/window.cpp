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


Window::Window(uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height) {

}

Window::~Window() {
    if (m_handle != nullptr) {
        glfwDestroyWindow(m_handle);
        m_handle = nullptr;
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

    m_handle = glfwCreateWindow(static_cast<int>(m_width), static_cast<int>(m_height), "RTGE", windowMonitor, nullptr);
    if (m_handle == nullptr) {
        error = "Failed to create window";
        return false;
    }

    if (!fullscreen) {
        centerWindow(monitor, mode, m_handle);
    }

    glfwSetInputMode(m_handle, GLFW_STICKY_KEYS, GLFW_TRUE);

    glfwMakeContextCurrent(m_handle);
    glfwSwapInterval(1);

    return true;
}

void Window::SetInputHandler(std::shared_ptr<InputHandler> handler) {
    m_inputHandler = handler;
}

bool Window::IsPressed(int key) const {
    return glfwGetKey(m_handle, key) == GLFW_PRESS;
}

void Window::Close() const {
    glfwSetWindowShouldClose(m_handle, GL_TRUE);
}

bool Window::StartFrame() {
    if (glfwWindowShouldClose(m_handle)) {
        return false;
    }

    return true;
}

void Window::EndFrame() {
    glfwSwapBuffers(m_handle);
    glfwPollEvents();
    if (m_inputHandler) {
        m_inputHandler->KeyHandler(this);
    }
}