#pragma once

#include <string>

struct GLFWwindow;

class Window {
public:
    Window() = delete;
    Window(uint32_t width, uint32_t height);
    ~Window();

public:
    bool Init(bool fullscreen, std::string& error);

private:
    uint32_t m_width = 800;
    uint32_t m_height = 600;
public:
    GLFWwindow* m_handle = nullptr;
};
