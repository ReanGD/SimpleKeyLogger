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

Key TranslateKeyCode(int key) {
    switch (key) {
    case GLFW_KEY_F1: return Key::F1;
    case GLFW_KEY_F2: return Key::F2;
    case GLFW_KEY_F3: return Key::F3;
    case GLFW_KEY_F4: return Key::F4;
    case GLFW_KEY_F5: return Key::F5;
    case GLFW_KEY_F6: return Key::F6;
    case GLFW_KEY_F7: return Key::F7;
    case GLFW_KEY_F8: return Key::F8;
    case GLFW_KEY_F9: return Key::F9;
    case GLFW_KEY_F10: return Key::F10;
    case GLFW_KEY_F11: return Key::F11;
    case GLFW_KEY_F12: return Key::F12;
    case GLFW_KEY_F13: return Key::F13;
    case GLFW_KEY_F14: return Key::F14;
    case GLFW_KEY_F15: return Key::F15;
    case GLFW_KEY_F16: return Key::F16;
    case GLFW_KEY_F17: return Key::F17;
    case GLFW_KEY_F18: return Key::F18;
    case GLFW_KEY_F19: return Key::F19;
    case GLFW_KEY_F20: return Key::F20;
    case GLFW_KEY_F21: return Key::F21;
    case GLFW_KEY_F22: return Key::F22;
    case GLFW_KEY_F23: return Key::F23;
    case GLFW_KEY_F24: return Key::F24;
    case GLFW_KEY_F25: return Key::F25;
    case GLFW_KEY_ESCAPE: return Key::Escape;
    case GLFW_KEY_ENTER: return Key::Enter;
    case GLFW_KEY_TAB: return Key::Tab;
    case GLFW_KEY_BACKSPACE: return Key::Backspace;
    case GLFW_KEY_CAPS_LOCK: return Key::CapsLock;
    case GLFW_KEY_SCROLL_LOCK: return Key::ScrollLock;
    case GLFW_KEY_NUM_LOCK: return Key::NumLock;
    case GLFW_KEY_SPACE: return Key::Space;
    case GLFW_KEY_INSERT: return Key::Insert;
    case GLFW_KEY_DELETE: return Key::Delete;
    case GLFW_KEY_PAGE_UP: return Key::PageUp;
    case GLFW_KEY_PAGE_DOWN: return Key::PageDown;
    case GLFW_KEY_HOME: return Key::Home;
    case GLFW_KEY_END: return Key::End;
    case GLFW_KEY_APOSTROPHE: return Key::Apostrophe;
    case GLFW_KEY_RIGHT: return Key::ArrowRight;
    case GLFW_KEY_LEFT: return Key::ArrowLeft;
    case GLFW_KEY_DOWN: return Key::ArrowDown;
    case GLFW_KEY_UP: return Key::ArrowUp;
    case GLFW_KEY_COMMA: return Key::Comma;
    case GLFW_KEY_MINUS: return Key::Minus;
    case GLFW_KEY_PERIOD: return Key::Period;
    case GLFW_KEY_SLASH: return Key::Slash;
    case GLFW_KEY_0: return Key::Digit0;
    case GLFW_KEY_1: return Key::Digit1;
    case GLFW_KEY_2: return Key::Digit2;
    case GLFW_KEY_3: return Key::Digit3;
    case GLFW_KEY_4: return Key::Digit4;
    case GLFW_KEY_5: return Key::Digit5;
    case GLFW_KEY_6: return Key::Digit6;
    case GLFW_KEY_7: return Key::Digit7;
    case GLFW_KEY_8: return Key::Digit8;
    case GLFW_KEY_9: return Key::Digit9;
    case GLFW_KEY_PRINT_SCREEN: return Key::PrintScreen;
    case GLFW_KEY_SEMICOLON: return Key::Semicolon;
    case GLFW_KEY_PAUSE: return Key::Pause;
    case GLFW_KEY_EQUAL: return Key::Equal;
    case GLFW_KEY_WORLD_1: return Key::NonUS1;
    case GLFW_KEY_WORLD_2: return Key::NonUS2;
    case GLFW_KEY_MENU: return Key::Menu;
    case GLFW_KEY_A: return Key::A;
    case GLFW_KEY_B: return Key::B;
    case GLFW_KEY_C: return Key::C;
    case GLFW_KEY_D: return Key::D;
    case GLFW_KEY_E: return Key::E;
    case GLFW_KEY_F: return Key::F;
    case GLFW_KEY_G: return Key::G;
    case GLFW_KEY_H: return Key::H;
    case GLFW_KEY_I: return Key::I;
    case GLFW_KEY_J: return Key::J;
    case GLFW_KEY_K: return Key::K;
    case GLFW_KEY_L: return Key::L;
    case GLFW_KEY_M: return Key::M;
    case GLFW_KEY_N: return Key::N;
    case GLFW_KEY_O: return Key::O;
    case GLFW_KEY_P: return Key::P;
    case GLFW_KEY_Q: return Key::Q;
    case GLFW_KEY_R: return Key::R;
    case GLFW_KEY_S: return Key::S;
    case GLFW_KEY_T: return Key::T;
    case GLFW_KEY_U: return Key::U;
    case GLFW_KEY_V: return Key::V;
    case GLFW_KEY_W: return Key::W;
    case GLFW_KEY_X: return Key::X;
    case GLFW_KEY_Y: return Key::Y;
    case GLFW_KEY_Z: return Key::Z;
    case GLFW_KEY_LEFT_BRACKET: return Key::LeftBracket;
    case GLFW_KEY_BACKSLASH: return Key::Backslash;
    case GLFW_KEY_RIGHT_BRACKET: return Key::RightBracket;
    case GLFW_KEY_GRAVE_ACCENT: return Key::GraveAccent;
    case GLFW_KEY_KP_0: return Key::KeyPad0;
    case GLFW_KEY_KP_1: return Key::KeyPad1;
    case GLFW_KEY_KP_2: return Key::KeyPad2;
    case GLFW_KEY_KP_3: return Key::KeyPad3;
    case GLFW_KEY_KP_4: return Key::KeyPad4;
    case GLFW_KEY_KP_5: return Key::KeyPad5;
    case GLFW_KEY_KP_6: return Key::KeyPad6;
    case GLFW_KEY_KP_7: return Key::KeyPad7;
    case GLFW_KEY_KP_8: return Key::KeyPad8;
    case GLFW_KEY_KP_9: return Key::KeyPad9;
    case GLFW_KEY_KP_DECIMAL: return Key::KeyPadDecimal;
    case GLFW_KEY_KP_DIVIDE: return Key::KeyPadDivide;
    case GLFW_KEY_KP_MULTIPLY: return Key::KeyPadMultiply;
    case GLFW_KEY_KP_SUBTRACT: return Key::KeyPadSubtract;
    case GLFW_KEY_KP_ADD: return Key::KeyPadAdd;
    case GLFW_KEY_KP_ENTER: return Key::KeyPadEnter;
    case GLFW_KEY_KP_EQUAL: return Key::KeyPadEqual;
    case GLFW_KEY_LEFT_SHIFT: return Key::LeftShift;
    case GLFW_KEY_LEFT_CONTROL: return Key::LeftControl;
    case GLFW_KEY_LEFT_ALT: return Key::LeftAlt;
    case GLFW_KEY_LEFT_SUPER: return Key::LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT: return Key::RightShift;
    case GLFW_KEY_RIGHT_CONTROL: return Key::RightControl;
    case GLFW_KEY_RIGHT_ALT: return Key::RightAlt;
    case GLFW_KEY_RIGHT_SUPER: return Key::RightSuper;
    default: return Key::Unknown;
    }
}

Key TranslateMouseKeyCode(int key) {
    switch (key) {
    case GLFW_MOUSE_BUTTON_LEFT: return Key::MouseLeft;
    case GLFW_MOUSE_BUTTON_RIGHT: return Key::MouseRight;
    case GLFW_MOUSE_BUTTON_MIDDLE: return Key::MouseMiddle;
    default: return Key::Unknown;
    }
}

struct GLFWCallbacks {
    static GLFWmousebuttonfun m_prevMouseButton;
    static void MouseButton(GLFWwindow* window, int key, int action, int mods) {
        if (m_prevMouseButton != nullptr) {
            m_prevMouseButton(window, key, action, mods);
        }
        auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
        w->GetIO().OnMouseKeyEvent(TranslateMouseKeyCode(key), static_cast<KeyAction>(action), static_cast<uint8_t>(mods));
    }

    static GLFWscrollfun m_prevScroll;
    static void Scroll(GLFWwindow* window, double offsetX, double offsetY) {
        if (m_prevScroll != nullptr) {
            m_prevScroll(window, offsetX, offsetY);
        }
        auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
        w->GetIO().OnScrollEvent(offsetX, offsetY);
    }

    static GLFWkeyfun m_prevKey;
    static void Key(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (m_prevKey != nullptr) {
            m_prevKey(window, key, scancode, action, mods);
        }
        auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
        w->GetIO().OnKeyEvent(TranslateKeyCode(key), static_cast<KeyAction>(action), static_cast<uint8_t>(mods));
    }

    static GLFWcharfun m_prevChar;
    static void Char(GLFWwindow* window, unsigned int ch) {
        if (m_prevChar != nullptr) {
            m_prevChar(window, ch);
        }
        auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
        w->GetIO().OnCharEvent(static_cast<char16_t>(ch));
    }

    static void SetAll(GLFWwindow* window) {
        m_prevMouseButton = glfwSetMouseButtonCallback(window, MouseButton);
        m_prevScroll = glfwSetScrollCallback(window, Scroll);
        m_prevKey = glfwSetKeyCallback(window, Key);
        m_prevChar = glfwSetCharCallback(window, Char);
    }

    static void ResetAll(GLFWwindow* window) {
        glfwSetMouseButtonCallback(window, m_prevMouseButton);
        m_prevMouseButton = nullptr;

        glfwSetScrollCallback(window, m_prevScroll);
        m_prevScroll = nullptr;

        glfwSetKeyCallback(window, m_prevKey);
        m_prevKey = nullptr;

        glfwSetCharCallback(window, m_prevChar);
        m_prevChar = nullptr;
    }
};

GLFWmousebuttonfun GLFWCallbacks::m_prevMouseButton = nullptr;
GLFWscrollfun GLFWCallbacks::m_prevScroll = nullptr;
GLFWkeyfun GLFWCallbacks::m_prevKey = nullptr;
GLFWcharfun GLFWCallbacks::m_prevChar = nullptr;


Window::~Window() {
    if (m_window != nullptr) {
        for (int i=0; i!=LastStandartCursor+1; ++i) {
            glfwDestroyCursor(m_cursors[i]);
            m_cursors[i] = nullptr;
        }

        GLFWCallbacks::ResetAll(m_window);
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

    m_cursors[CursorType::Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_cursors[CursorType::IBeam] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    m_cursors[CursorType::Crosshair] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    m_cursors[CursorType::Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    m_cursors[CursorType::ResizeH] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    m_cursors[CursorType::ResizeV] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_currentCursor = CursorType::Disabled;

    // if (glfwRawMouseMotionSupported()) {
    //     glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    // }

    GLFWCallbacks::SetAll(m_window);

    return true;
}

void Window::Close() {
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

bool Window::StartFrame() {
    if (glfwWindowShouldClose(m_window)) {
        return false;
    }

    return true;
}

void Window::EndFrame() {
    glfwSwapBuffers(m_window);

    double cursorPosX, cursorPosY;
    glfwGetCursorPos(m_window, &cursorPosX, &cursorPosY);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
    m_io.Update(static_cast<uint32_t>(fbWidth), static_cast<uint32_t>(fbHeight), cursorPosX, cursorPosY);
    glfwPollEvents();
}

void Window::SetCursor(CursorType value) {
    if (m_currentCursor == value) {
        return;
    }
    switch (value) {
    case CursorType::Arrow:
    case CursorType::IBeam:
    case CursorType::Crosshair:
    case CursorType::Hand:
    case CursorType::ResizeH:
    case CursorType::ResizeV:
        glfwSetCursor(m_window, m_cursors[value]);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_currentCursor = value;
        break;

    case CursorType::Hidden:
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        m_currentCursor = value;
        break;

    case CursorType::Disabled:
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_currentCursor = value;
        break;
    }
}

void Window::SetCursorPosition(float posX, float posY) {
    glfwSetCursorPos(m_window, static_cast<double>(posX), static_cast<double>(posY));
}

bool Window::IsFocused() {
    return glfwGetWindowAttrib(m_window, GLFW_FOCUSED) != 0;
}

void Window::GetWindowSize(uint32_t& width, uint32_t& height) {
    int w, h;
    glfwGetWindowSize(m_window, &w, &h);
    width = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);
}

WindowInput& Window::GetIO() {
    return m_io;
}
