#pragma once

#include <string>
#include <memory>

struct GLFWwindow;
struct InputHandler {
    struct Executor {
        Executor() = delete;
        Executor(const Executor&) = delete;
        Executor(Executor&&) = delete;
        Executor& operator=(const Executor&) = delete;
        Executor& operator=(Executor&&) = delete;

        Executor(GLFWwindow* handle);

        void Close() const;
        bool IsPressed(int key) const;
    private:
        GLFWwindow* m_handle = nullptr;
    };

    InputHandler() = default;
    virtual ~InputHandler() = default;
    virtual void KeyHandler(const Executor& e) = 0;
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
    void SetInputHandler(std::weak_ptr<InputHandler> handler);
    bool StartFrame();
    void EndFrame();

private:
    uint32_t m_width = 800;
    uint32_t m_height = 600;

    std::weak_ptr<InputHandler> m_inputHandler;
public:
    GLFWwindow* m_window = nullptr;
};
