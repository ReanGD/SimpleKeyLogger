#pragma once

#include <vector>
#include <memory>

#include "window/window_input.h"


enum CursorType : uint8_t {
    Arrow = 0,      // GLFW_ARROW_CURSOR
    IBeam = 1,      // GLFW_IBEAM_CURSOR
    Crosshair = 2,  // GLFW_CROSSHAIR_CURSOR
    Hand = 3,       // GLFW_HAND_CURSOR
    ResizeH = 4,    // GLFW_HRESIZE_CURSOR
    ResizeV = 5,    // GLFW_VRESIZE_CURSOR

    Hidden = 6,
    Disabled = 7,

    LastStandartCursor = ResizeV,
};

struct GLFWwindow;
struct GLFWcursor;
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
    void Close();

    bool StartFrame();
    void EndFrame();

    void SetCursor(CursorType value);
    void SetCursorPosition(float posX, float posY);

    bool IsFocused();
    void GetWindowSize(uint32_t& width, uint32_t& height);

    const char* GetClipboardText();
    void SetClipboardText(const char* text);
    static const char* GetClipboardText(void*);
    static void SetClipboardText(void*, const char* text);

    WindowInput& GetIO();
private:
    WindowInput m_io;
    CursorType m_currentCursor = CursorType::Disabled;
    GLFWcursor* m_cursors[static_cast<size_t>(CursorType::LastStandartCursor) + 1] = { nullptr };
    GLFWwindow* m_window = nullptr;
};
