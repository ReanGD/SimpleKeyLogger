#pragma once

#include <string>
#include <memory>

struct GLFWwindow;
struct InputHandler {
    enum class Key : uint16_t {
        SPACE         = 32,
        APOSTROPHE    = 39,  /* ' */
        COMMA         = 44,  /* , */
        MINUS         = 45,  /* - */
        PERIOD        = 46,  /* . */
        SLASH         = 47,  /* / */
        Number0       = 48,
        Number1       = 49,
        Number2       = 50,
        Number3       = 51,
        Number4       = 52,
        Number5       = 53,
        Number6       = 54,
        Number7       = 55,
        Number8       = 56,
        Number9       = 57,
        SEMICOLON     = 59,  /* ; */
        EQUAL         = 61,  /* = */
        A             = 65,
        B             = 66,
        C             = 67,
        D             = 68,
        E             = 69,
        F             = 70,
        G             = 71,
        H             = 72,
        I             = 73,
        J             = 74,
        K             = 75,
        L             = 76,
        M             = 77,
        N             = 78,
        O             = 79,
        P             = 80,
        Q             = 81,
        R             = 82,
        S             = 83,
        T             = 84,
        U             = 85,
        V             = 86,
        W             = 87,
        X             = 88,
        Y             = 89,
        Z             = 90,
        LEFT_BRACKET  = 91,  /* [ */
        BACKSLASH     = 92,  /* \ */
        RIGHT_BRACKET = 93,  /* ] */
        GRAVE_ACCENT  = 96,  /* ` */
        WORLD_1       = 161, /* non-US #1 */
        WORLD_2       = 162, /* non-US #2 */
        ESCAPE        = 256,
        ENTER         = 257,
        TAB           = 258,
        BACKSPACE     = 259,
        INSERT        = 260,
        DELETE        = 261,
        RIGHT         = 262,
        LEFT          = 263,
        DOWN          = 264,
        UP            = 265,
        PAGE_UP       = 266,
        PAGE_DOWN     = 267,
        HOME          = 268,
        END           = 269,
        CAPS_LOCK     = 280,
        SCROLL_LOCK   = 281,
        NUM_LOCK      = 282,
        PRINT_SCREEN  = 283,
        PAUSE         = 284,
        F1            = 290,
        F2            = 291,
        F3            = 292,
        F4            = 293,
        F5            = 294,
        F6            = 295,
        F7            = 296,
        F8            = 297,
        F9            = 298,
        F10           = 299,
        F11           = 300,
        F12           = 301,
        F13           = 302,
        F14           = 303,
        F15           = 304,
        F16           = 305,
        F17           = 306,
        F18           = 307,
        F19           = 308,
        F20           = 309,
        F21           = 310,
        F22           = 311,
        F23           = 312,
        F24           = 313,
        F25           = 314,
        KP_0          = 320,
        KP_1          = 321,
        KP_2          = 322,
        KP_3          = 323,
        KP_4          = 324,
        KP_5          = 325,
        KP_6          = 326,
        KP_7          = 327,
        KP_8          = 328,
        KP_9          = 329,
        KP_DECIMAL    = 330,
        KP_DIVIDE     = 331,
        KP_MULTIPLY   = 332,
        KP_SUBTRACT   = 333,
        KP_ADD        = 334,
        KP_ENTER      = 335,
        KP_EQUAL      = 336,
        LEFT_SHIFT    = 340,
        LEFT_CONTROL  = 341,
        LEFT_ALT      = 342,
        LEFT_SUPER    = 343,
        RIGHT_SHIFT   = 344,
        RIGHT_CONTROL = 345,
        RIGHT_ALT     = 346,
        RIGHT_SUPER   = 347,
        MENU          = 348,
    };

    struct Executor {
        Executor() = delete;
        Executor(const Executor&) = delete;
        Executor(Executor&&) = delete;
        Executor& operator=(const Executor&) = delete;
        Executor& operator=(Executor&&) = delete;

        Executor(GLFWwindow* handle);

        void Close() const;
        bool IsPressed(Key value) const;
    private:
        GLFWwindow* m_handle = nullptr;
    };

    InputHandler() = default;
    virtual ~InputHandler() = default;
    virtual void KeyHandler(const Executor& e) = 0;
    virtual void MouseHandler(float dtX, float dtY) = 0;
    virtual void ScreenHandler(uint32_t width, uint32_t height) = 0;
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

    void OnFramebufferSizeChanged(int width, int height);
    void OnCursorPositionChanged(double posX, double posY);
private:
    uint32_t m_width = 800;
    uint32_t m_height = 600;
    double m_cursorX = 0;
    double m_cursorY = 0;

    std::weak_ptr<InputHandler> m_inputHandler;
    GLFWwindow* m_window = nullptr;
};
