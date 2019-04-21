#pragma once

#include <string>
#include <memory>

struct GLFWwindow;
class Window;

struct InputHandler {
    virtual void KeyHandler(const Window* window) = 0;
    virtual void MouseHandler(float dtX, float dtY) = 0;
};

class Window {
public:
    Window() = delete;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    Window(uint32_t width, uint32_t height);
    ~Window();

public:
    bool Init(bool fullscreen, std::string& error);
    void SetInputHandler(std::shared_ptr<InputHandler> handler);
    bool IsPressed(int key) const;
    void Close() const;
    bool StartFrame();
    void EndFrame();

private:
    uint32_t m_width = 800;
    uint32_t m_height = 600;

    std::shared_ptr<InputHandler> m_inputHandler = nullptr;
public:
    GLFWwindow* m_handle = nullptr;
};
