#include "window/user_input.h"


static constexpr const uint8_t IsDownMask = 1 << 4;
static constexpr const uint8_t IsStickyDownMask = 1 << 5;
static constexpr const uint8_t FirstPressMask = 1 << 6;
static constexpr const uint8_t FirstReleaseMask = 1 << 7;

void UserInput::GetCursorPosition(float& posX, float& posY) {
    posX = static_cast<float>(m_cursorPosX);
    posY = static_cast<float>(m_cursorPosY);
}

void UserInput::GetCursorOffet(float& offsetX, float& offsetY) {
    offsetX = static_cast<float>(m_cursorLastPosX - m_cursorPosX);
    offsetY = static_cast<float>(m_cursorLastPosY - m_cursorPosY);
}

float UserInput::GetScrollOffsetY() {
    return static_cast<float>(m_scrollOffsetY);
}

void UserInput::GetScrollOffset(float& offsetX, float& offsetY) {
    offsetX = static_cast<float>(m_scrollOffsetX);
    offsetY = static_cast<float>(m_scrollOffsetY);
}

bool UserInput::IsKeyPressedFirstTime(Key code) {
    if (code > Key::Last) {
        return false;
    }

    return (m_isKeyDown[static_cast<size_t>(code)] & FirstPressMask) != 0;
}

bool UserInput::IsKeyReleasedFirstTime(Key code) {
    if (code > Key::Last) {
        return false;
    }

    return (m_isKeyDown[static_cast<size_t>(code)] & FirstReleaseMask) != 0;
}

bool UserInput::IsKeyDown(Key code) {
    if (code > Key::Last) {
        return false;
    }

    return (m_isKeyDown[static_cast<size_t>(code)] & IsDownMask) != 0;
}

bool UserInput::IsKeyStickyDown(Key code) {
    if (code > Key::Last) {
        return false;
    }

    return (m_isKeyDown[static_cast<size_t>(code)] & IsStickyDownMask) != 0;
}

bool UserInput::GetKeyStickyDownState(Key code, uint8_t& mods) {
    if (code > Key::Last) {
        return false;
    }

    uint8_t state = m_isKeyDown[static_cast<size_t>(code)];
    mods = state & KeyModifier::Mask;
    return (state & IsStickyDownMask) != 0;
}

std::u16string UserInput::GetInput() {
    return m_userInput;
}

void UserInput::Update(double cursorPosX, double cursorPosY) {
    m_cursorLastPosX = m_cursorPosX;
    m_cursorLastPosY = m_cursorPosY;
    m_cursorPosX = cursorPosX;
    m_cursorPosY = cursorPosY;
    m_scrollOffsetX = 0;
    m_scrollOffsetY = 0;
    if (m_firstUpdate) {
        m_firstUpdate = false;
        m_cursorLastPosX = m_cursorPosX;
        m_cursorLastPosY = m_cursorPosY;
    }

    m_userInput.clear();
    for(size_t i=0; i!=(static_cast<size_t>(Key::Last) + 1); ++i) {
        if ((m_isKeyDown[i] & IsDownMask) != 0) {
            // clear FirstPress and FirstRelease bits
            m_isKeyDown[i] &= ~(FirstPressMask | FirstReleaseMask);
        } else {
            m_isKeyDown[i] = 0;
        }
    }
}

void UserInput::OnScrollEvent(double offsetX, double offsetY) {
    m_scrollOffsetX += offsetX;
    m_scrollOffsetY += offsetY;
}

void UserInput::OnKeyEvent(Key code, KeyAction action, uint8_t mods) {
    if ((code > Key::LastKeyboard) || (action > KeyAction::Last)) {
        return;
    }

    uint8_t state = m_isKeyDown[static_cast<size_t>(code)] | (mods & KeyModifier::Mask);
    switch (action)
    {
    case KeyAction::Release:
        state |= FirstReleaseMask;
        // clear KeyModifier bits and IsDownMask bit
        state &= ~(KeyModifier::Mask | IsDownMask);
        break;
    case KeyAction::Press:
        state |= FirstPressMask;
        state |= IsStickyDownMask;
        state |= IsDownMask;
        break;
    case KeyAction::Repeat:
        state |= IsStickyDownMask;
        state |= IsDownMask;
        break;
    }
    m_isKeyDown[static_cast<size_t>(code)] = state;

    // modifier key processing
    switch (code)
    {
    case Key::LeftShift:
    case Key::RightShift:
        m_isKeyDown[static_cast<size_t>(Key::Shift)] = state;
        break;

    case Key::LeftControl:
    case Key::RightControl:
        m_isKeyDown[static_cast<size_t>(Key::Control)] = state;
        break;

    case Key::LeftAlt:
    case Key::RightAlt:
        m_isKeyDown[static_cast<size_t>(Key::Alt)] = state;
        break;

    case Key::LeftSuper:
    case Key::RightSuper:
        m_isKeyDown[static_cast<size_t>(Key::Super)] = state;
        break;

    default:
        break;
    }

}

void UserInput::OnMouseKeyEvent(Key code, KeyAction action, uint8_t mods) {
    if ((code < Key::FirstMouse) || (code > Key::LastMouse)  || ((action != KeyAction::Release) && (action != KeyAction::Press))) {
        return;
    }

    uint8_t state = m_isKeyDown[static_cast<size_t>(code)] | (mods & KeyModifier::Mask);
    switch (action)
    {
    case KeyAction::Release:
        state |= FirstReleaseMask;
        // clear KeyModifier bits and IsDownMask bit
        state &= ~(KeyModifier::Mask | IsDownMask);
        break;
    case KeyAction::Press:
        state |= FirstPressMask;
        state |= IsStickyDownMask;
        state |= IsDownMask;
        break;
    case KeyAction::Repeat:
        break;
    }
    m_isKeyDown[static_cast<size_t>(code)] = state;
}

void UserInput::OnCharEvent(char16_t ch) {
    m_userInput += ch;
}
