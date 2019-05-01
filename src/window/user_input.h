#pragma once

#include <string>

enum class Key : uint8_t {
        F1             = 0,            // GLFW_KEY_F1
        F2             = 1,            // GLFW_KEY_F2
        F3             = 2,            // GLFW_KEY_F3
        F4             = 3,            // GLFW_KEY_F4
        F5             = 4,            // GLFW_KEY_F5
        F6             = 5,            // GLFW_KEY_F6
        F7             = 6,            // GLFW_KEY_F7
        F8             = 7,            // GLFW_KEY_F8
        F9             = 8,            // GLFW_KEY_F9
        F10            = 9,            // GLFW_KEY_F10
        F11            = 10,           // GLFW_KEY_F11
        F12            = 11,           // GLFW_KEY_F12
        F13            = 12,           // GLFW_KEY_F13
        F14            = 13,           // GLFW_KEY_F14
        F15            = 14,           // GLFW_KEY_F15
        F16            = 15,           // GLFW_KEY_F16
        F17            = 16,           // GLFW_KEY_F17
        F18            = 17,           // GLFW_KEY_F18
        F19            = 18,           // GLFW_KEY_F19
        F20            = 19,           // GLFW_KEY_F20
        F21            = 20,           // GLFW_KEY_F21
        F22            = 21,           // GLFW_KEY_F22
        F23            = 22,           // GLFW_KEY_F23
        F24            = 23,           // GLFW_KEY_F24
        F25            = 24,           // GLFW_KEY_F25
        Escape         = 25,           // GLFW_KEY_ESCAPE
        Enter          = 26,           // GLFW_KEY_ENTER
        Tab            = 27,           // GLFW_KEY_TAB
        Backspace      = 28,           // GLFW_KEY_BACKSPACE
        CapsLock       = 29,           // GLFW_KEY_CAPS_LOCK
        ScrollLock     = 30,           // GLFW_KEY_SCROLL_LOCK
        NumLock        = 31,           // GLFW_KEY_NUM_LOCK
        Space          = 32,           // ascii, GLFW_KEY_SPACE
        Insert         = 33,           // GLFW_KEY_INSERT
        Delete         = 34,           // GLFW_KEY_DELETE
        PageUp         = 35,           // GLFW_KEY_PAGE_UP
        PageDown       = 36,           // GLFW_KEY_PAGE_DOWN
        Home           = 37,           // GLFW_KEY_HOME
        End            = 38,           // GLFW_KEY_END
        Apostrophe     = 39,  /* ' */  // ascii, GLFW_KEY_APOSTROPHE
        ArrowRight     = 40,           // GLFW_KEY_RIGHT
        ArrowLeft      = 41,           // GLFW_KEY_LEFT
        ArrowDown      = 42,           // GLFW_KEY_DOWN
        ArrowUp        = 43,           // GLFW_KEY_UP
        Comma          = 44,  /* , */  // ascii, GLFW_KEY_COMMA
        Minus          = 45,  /* - */  // ascii, GLFW_KEY_MINUS
        Period         = 46,  /* . */  // ascii, GLFW_KEY_PERIOD
        Slash          = 47,  /* / */  // ascii, GLFW_KEY_SLASH
        Digit0         = 48,           // ascii, GLFW_KEY_0
        Digit1         = 49,           // ascii, GLFW_KEY_1
        Digit2         = 50,           // ascii, GLFW_KEY_2
        Digit3         = 51,           // ascii, GLFW_KEY_3
        Digit4         = 52,           // ascii, GLFW_KEY_4
        Digit5         = 53,           // ascii, GLFW_KEY_5
        Digit6         = 54,           // ascii, GLFW_KEY_6
        Digit7         = 55,           // ascii, GLFW_KEY_7
        Digit8         = 56,           // ascii, GLFW_KEY_8
        Digit9         = 57,           // ascii, GLFW_KEY_9
        PrintScreen    = 58,           // GLFW_KEY_PRINT_SCREEN
        Semicolon      = 59,  /* ; */  // ascii, GLFW_KEY_SEMICOLON
        Pause          = 60,           // GLFW_KEY_PAUSE
        Equal          = 61,  /* = */  // ascii, GLFW_KEY_EQUAL
        NonUS1         = 62,           // GLFW_KEY_WORLD_1
        NonUS2         = 63,           // GLFW_KEY_WORLD_2
        Menu           = 64,           // GLFW_KEY_MENU
        A              = 65,           // ascii, GLFW_KEY_A
        B              = 66,           // ascii, GLFW_KEY_B
        C              = 67,           // ascii, GLFW_KEY_C
        D              = 68,           // ascii, GLFW_KEY_D
        E              = 69,           // ascii, GLFW_KEY_E
        F              = 70,           // ascii, GLFW_KEY_F
        G              = 71,           // ascii, GLFW_KEY_G
        H              = 72,           // ascii, GLFW_KEY_H
        I              = 73,           // ascii, GLFW_KEY_I
        J              = 74,           // ascii, GLFW_KEY_J
        K              = 75,           // ascii, GLFW_KEY_K
        L              = 76,           // ascii, GLFW_KEY_L
        M              = 77,           // ascii, GLFW_KEY_M
        N              = 78,           // ascii, GLFW_KEY_N
        O              = 79,           // ascii, GLFW_KEY_O
        P              = 80,           // ascii, GLFW_KEY_P
        Q              = 81,           // ascii, GLFW_KEY_Q
        R              = 82,           // ascii, GLFW_KEY_R
        S              = 83,           // ascii, GLFW_KEY_S
        T              = 84,           // ascii, GLFW_KEY_T
        U              = 85,           // ascii, GLFW_KEY_U
        V              = 86,           // ascii, GLFW_KEY_V
        W              = 87,           // ascii, GLFW_KEY_W
        X              = 88,           // ascii, GLFW_KEY_X
        Y              = 89,           // ascii, GLFW_KEY_Y
        Z              = 90,           // ascii, GLFW_KEY_Z
        LeftBracket    = 91,  /* [ */  // ascii, GLFW_KEY_LEFT_BRACKET
        Backslash      = 92,  /* \ */  // ascii, GLFW_KEY_BACKSLASH
        RightBracket   = 93,  /* ] */  // ascii, GLFW_KEY_RIGHT_BRACKET
        GraveAccent    = 94,  /* ` */  // GLFW_KEY_GRAVE_ACCENT
        KeyPad0        = 95,           // GLFW_KEY_KP_0
        KeyPad1        = 96,           // GLFW_KEY_KP_1
        KeyPad2        = 97,           // GLFW_KEY_KP_2
        KeyPad3        = 98,           // GLFW_KEY_KP_3
        KeyPad4        = 99,           // GLFW_KEY_KP_4
        KeyPad5        = 100,          // GLFW_KEY_KP_5
        KeyPad6        = 101,          // GLFW_KEY_KP_6
        KeyPad7        = 102,          // GLFW_KEY_KP_7
        KeyPad8        = 103,          // GLFW_KEY_KP_8
        KeyPad9        = 104,          // GLFW_KEY_KP_9
        KeyPadDecimal  = 105,          // GLFW_KEY_KP_DECIMAL
        KeyPadDivide   = 106,          // GLFW_KEY_KP_DIVIDE
        KeyPadMultiply = 107,          // GLFW_KEY_KP_MULTIPLY
        KeyPadSubtract = 108,          // GLFW_KEY_KP_SUBTRACT
        KeyPadAdd      = 109,          // GLFW_KEY_KP_ADD
        KeyPadEnter    = 110,          // GLFW_KEY_KP_ENTER
        KeyPadEqual    = 111,          // GLFW_KEY_KP_EQUAL
        LeftShift      = 112,          // GLFW_KEY_LEFT_SHIFT
        LeftControl    = 113,          // GLFW_KEY_LEFT_CONTROL
        LeftAlt        = 114,          // GLFW_KEY_LEFT_ALT
        LeftSuper      = 115,          // GLFW_KEY_LEFT_SUPER
        RightShift     = 116,          // GLFW_KEY_RIGHT_SHIFT
        RightControl   = 117,          // GLFW_KEY_RIGHT_CONTROL
        RightAlt       = 118,          // GLFW_KEY_RIGHT_ALT
        RightSuper     = 119,          // GLFW_KEY_RIGHT_SUPER

        // Virtual
        Shift          = 120,          // GLFW_KEY_LEFT_SHIFT || GLFW_KEY_RIGHT_SHIFT
        Control        = 121,          // GLFW_KEY_LEFT_CONTROL || GLFW_KEY_RIGHT_CONTROL
        Alt            = 122,          // GLFW_KEY_LEFT_ALT || GLFW_KEY_RIGHT_ALT
        Super          = 123,          // GLFW_KEY_LEFT_SUPER || GLFW_KEY_RIGHT_SUPER

        // Mouse
        MouseLeft      = 124,          // GLFW_MOUSE_BUTTON_LEFT
        MouseRight     = 125,          // GLFW_MOUSE_BUTTON_RIGHT
        MouseMiddle    = 126,          // GLFW_MOUSE_BUTTON_MIDDLE

        Unknown        = 127,

        FirstMod       = LeftShift,
        LastKeyboard   = RightSuper,
        FirstMouse     = MouseLeft,
        LastMouse      = MouseMiddle,
        Last           = MouseMiddle,
};

enum KeyModifier : uint8_t {
    Shift   = 1,  // GLFW_MOD_SHIFT
    Control = 2,  // GLFW_MOD_CONTROL
    Alt     = 4,  // GLFW_MOD_ALT
    Super   = 8,  // GLFW_MOD_SUPER
    Mask    = Shift | Control | Alt | Super,
};

enum KeyAction : uint8_t {
    Release = 0,  // GLFW_RELEASE
    Press   = 1,  // GLFW_PRESS
    Repeat  = 2,  // GLFW_REPEAT
    Last    = Repeat,
};

class UserInput {
public:
    UserInput() = default;
    ~UserInput() = default;
    UserInput(const UserInput&) = delete;
    UserInput(UserInput&&) = delete;
    UserInput& operator=(const UserInput&) = delete;
    UserInput& operator=(UserInput&&) = delete;

public:
    void GetCursorPosition(float& posX, float& posY);
    void GetCursorOffet(float& offsetX, float& offsetY);

    float GetScrollOffsetY();
    void GetScrollOffset(float& offsetX, float& offsetY);

    bool IsKeyPressedFirstTime(Key code);
    bool IsKeyReleasedFirstTime(Key code);
    bool IsKeyDown(Key code);
    bool GetKeyDownState(Key code, uint8_t& mods);

    std::u16string GetInput();

    void Update(double cursorPosX, double cursorPosY);
    void OnScrollEvent(double offsetX, double offsetY);
    void OnKeyEvent(Key code, KeyAction action, uint8_t mods);
    void OnMouseKeyEvent(Key code, KeyAction action, uint8_t mods);
    void OnCharEvent(char16_t ch);

private:
    bool m_firstUpdate = true;
    double m_cursorPosX = 0;
    double m_cursorPosY = 0;
    double m_cursorLastPosX = 0;
    double m_cursorLastPosY = 0;
    double m_scrollOffsetX = 0;
    double m_scrollOffsetY = 0;
    // FirstReleaseMask, FirstPress, IsDownMask, Super, Alt, Control, Shift
    uint8_t m_isKeyDown[static_cast<size_t>(Key::Last) + 1] = { 0 };
    std::u16string m_userInput;
};
