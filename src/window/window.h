#pragma once

#include <vector>
#include <memory>

#include "window/window_input.h"

struct GLFWwindow;
enum ProcessMode : uint8_t {
    Editor = 1 << 0,
    FirstPerson = 1 << 1,
};

class Window {
public:
    Window() = default;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;
    ~Window();

public:
    bool Init(bool fullscreen, float windowMultiplier, std::string& error);
    bool StartFrame();
    void EndFrame();
    void Close();

    WindowInput& GetIO();

    uint8_t EditorModeInverse();
private:
    uint8_t m_mode = ProcessMode::Editor;
    WindowInput m_io;
    GLFWwindow* m_window = nullptr;
};
