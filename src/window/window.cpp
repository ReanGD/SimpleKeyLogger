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

GLFWwindow* createWindow(bool fullscreen, float windowMultiplier, std::string& error) {
    GLFWwindow* window = nullptr;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor == nullptr) {
        error = "Failed to get primary monitor";
        return window;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (mode == nullptr) {
        error = "Failed to get monitor mode";
        return window;
    }

    int width = mode->width;
    int height = mode->height;
    auto windowMonitor = monitor;
    if (!fullscreen) {
        windowMonitor = nullptr;
        width = static_cast<int>(windowMultiplier * static_cast<float>(width));
        height = static_cast<int>(windowMultiplier * static_cast<float>(height));
    }

    window = glfwCreateWindow(width, height, "RTGE", windowMonitor, nullptr);
    if (window == nullptr) {
        error = "Failed to create window";
        return window;
    }

    if (!fullscreen) {
        centerWindow(monitor, mode, window);
    }

    return window;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    static_cast<Window*>(glfwGetWindowUserPointer(window))->OnFramebufferSizeChanged(
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height));
}

void cursorPosCallback(GLFWwindow* window, double posX, double posY) {
    static_cast<Window*>(glfwGetWindowUserPointer(window))->OnCursorPositionChanged(posX, posY);
}

void keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key != GLFW_KEY_UNKNOWN) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->OnKeyPressed(
            static_cast<InputHandler::Key>(key),
            static_cast<InputHandler::Action>(action));
    }
}

InputHandler::Executor::Executor(GLFWwindow* handle)
    : m_handle(handle) {

}

bool InputHandler::Executor::IsPressed(Key value) const {
    return glfwGetKey(m_handle, static_cast<int>(value)) == GLFW_PRESS;
}

Window::~Window() {
    if (m_window != nullptr) {
        glfwSetCursorPosCallback(m_window, nullptr);
        glfwSetFramebufferSizeCallback(m_window, nullptr);
        glfwSetKeyCallback(m_window, nullptr);
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool Window::Init(bool fullscreen, float windowMultiplier, std::string& error) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = createWindow(fullscreen, windowMultiplier, error);
    if (m_window == nullptr) {
        return false;
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_CURSOR, ((m_mode & InputHandler::ProcessMode::Editor) != 0) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    glfwGetCursorPos(m_window, &m_cursorX, &m_cursorY);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);

    glfwSetKeyCallback(m_window, keyCallback);

    return true;
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
    for (auto& handlerWeak : m_inputHandlers) {
        if (auto handler = handlerWeak.lock(); handler && ((handler->m_supportMode & m_mode) != 0)) {
            handler->KeyHandler(InputHandler::Executor(m_window));
        }
    }
}

void Window::Close() {
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

void Window::AttachInputHandler(std::weak_ptr<InputHandler> handlerWeak) {
    if (auto handler = handlerWeak.lock(); handler) {
        m_inputHandlers.push_back(handler);

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        handler->ScreenHandler(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    }
}

void Window::EditorModeInverse() {
    if ((m_mode & InputHandler::ProcessMode::Editor) != 0) {
        m_mode = InputHandler::ProcessMode::FirstPerson;
    } else {
        m_mode = InputHandler::ProcessMode::Editor;
    }

    glfwSetInputMode(m_window, GLFW_CURSOR, ((m_mode & InputHandler::ProcessMode::Editor) != 0) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Window::OnFramebufferSizeChanged(uint32_t width, uint32_t height) {
    glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
    for (auto& handlerWeak : m_inputHandlers) {
        if (auto handler = handlerWeak.lock(); handler) {
            handler->ScreenHandler(width, height);
        }
    }
}

void Window::OnCursorPositionChanged(double posX, double posY) {
    float dtX = static_cast<float>(m_cursorX - posX);
    float dtY = static_cast<float>(m_cursorY - posY);
    m_cursorX = posX;
    m_cursorY = posY;

    for (auto& handlerWeak : m_inputHandlers) {
        if (auto handler = handlerWeak.lock(); handler && ((handler->m_supportMode & m_mode) != 0)) {
            handler->MouseHandler(dtX, dtY);
        }
    }
}

void Window::OnKeyPressed(InputHandler::Key key, InputHandler::Action action) {
    for (auto& handlerWeak : m_inputHandlers) {
        if (auto handler = handlerWeak.lock(); handler && ((handler->m_supportMode & m_mode) != 0)) {
            handler->KeyPressHandler(key, action);
        }
    }
}
