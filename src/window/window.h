#pragma once

#include <vector>
#include <memory>

#include "window/window_input.h"

struct GLFWwindow;
struct InputHandler {
    enum ProcessMode : uint8_t {
        Editor = 1 << 0,
        FirstPerson = 1 << 1,
    };

    virtual ~InputHandler() = default;
    virtual void ScreenHandler(uint32_t /*width*/,  uint32_t /*height*/) {}

    uint8_t m_supportMode = ProcessMode::Editor | ProcessMode::FirstPerson;
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

    void AttachInputHandler(std::weak_ptr<InputHandler> handlerWeak);
    uint8_t EditorModeInverse();

    void OnFramebufferSizeChanged(uint32_t width, uint32_t height);
private:
    uint8_t m_mode = InputHandler::ProcessMode::Editor;
    std::vector<std::weak_ptr<InputHandler>> m_inputHandlers;
    WindowInput m_io;
    GLFWwindow* m_window = nullptr;
};
