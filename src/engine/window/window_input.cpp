#include "engine/window/window_input.h"


static constexpr const uint8_t IsDownMask = 1 << 4;
static constexpr const uint8_t IsStickyDownMask = 1 << 5;
static constexpr const uint8_t FirstPressMask = 1 << 6;
static constexpr const uint8_t FirstReleaseMask = 1 << 7;

bool WindowInput::GetFramebufferSize(uint32_t& width, uint32_t& height) const noexcept {
    width = m_fbWidth;
    height = m_fbHeight;

    return m_fbChanged;
}

void WindowInput::GetCursorPosition(float& posX, float& posY) const noexcept {
    posX = static_cast<float>(m_cursorPosX);
    posY = static_cast<float>(m_cursorPosY);
}

void WindowInput::GetCursorOffet(float& offsetX, float& offsetY) const noexcept {
    offsetX = static_cast<float>(m_cursorLastPosX - m_cursorPosX);
    offsetY = static_cast<float>(m_cursorLastPosY - m_cursorPosY);
}

float WindowInput::GetScrollOffsetY() const noexcept {
    return static_cast<float>(m_scrollOffsetY);
}

void WindowInput::GetScrollOffset(float& offsetX, float& offsetY) const noexcept {
    offsetX = static_cast<float>(m_scrollOffsetX);
    offsetY = static_cast<float>(m_scrollOffsetY);
}

bool WindowInput::IsKeyPressedFirstTime(Key code) const noexcept {
    if (code > Key::Last) {
        return false;
    }

    return (m_isKeyDown[static_cast<size_t>(code)] & FirstPressMask) != 0;
}

bool WindowInput::IsKeyReleasedFirstTime(Key code) const noexcept {
    if (code > Key::Last) {
        return false;
    }

    return (m_isKeyDown[static_cast<size_t>(code)] & FirstReleaseMask) != 0;
}

bool WindowInput::IsKeyDown(Key code) const noexcept {
    if (code > Key::Last) {
        return false;
    }

    return (m_isKeyDown[static_cast<size_t>(code)] & IsDownMask) != 0;
}

bool WindowInput::IsKeyStickyDown(Key code) const noexcept {
    if (code > Key::Last) {
        return false;
    }

    return (m_isKeyDown[static_cast<size_t>(code)] & IsStickyDownMask) != 0;
}

bool WindowInput::GetKeyStickyDownState(Key code, uint8_t& mods) const noexcept {
    if (code > Key::Last) {
        return false;
    }

    uint8_t state = m_isKeyDown[static_cast<size_t>(code)];
    mods = state & KeyModifier::Mask;
    return (state & IsStickyDownMask) != 0;
}

void WindowInput::FillKeyboardKeysDown(bool* keysDown) const noexcept {
    if (keysDown == nullptr) {
        return;
    }
    for(size_t i=0; i!=(static_cast<size_t>(Key::LastKeyboard) + 1); ++i) {
        keysDown[i] = (m_isKeyDown[i] & IsDownMask) != 0;
    }
}

std::u16string WindowInput::GetInput() const noexcept {
    return m_userInput;
}

void WindowInput::Update() noexcept {
    m_scrollOffsetX = 0;
    m_scrollOffsetY = 0;

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

void WindowInput::OnFramebufferSizeEvent(uint32_t width, uint32_t height) noexcept {
    m_fbChanged = ((m_fbWidth != width) || (m_fbHeight != height));
    m_fbWidth = width;
    m_fbHeight = height;
}

void WindowInput::OnCursorEvent(double posX, double posY) noexcept {
    m_cursorLastPosX = m_cursorPosX;
    m_cursorLastPosY = m_cursorPosY;
    m_cursorPosX = posX;
    m_cursorPosY = posY;
}

void WindowInput::OnScrollEvent(double offsetX, double offsetY) noexcept {
    m_scrollOffsetX += offsetX;
    m_scrollOffsetY += offsetY;
}

void WindowInput::OnKeyEvent(Key code, KeyAction action, uint8_t mods) noexcept {
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
        m_isKeyDown[static_cast<size_t>(Key::Shift)] = m_isKeyDown[static_cast<size_t>(Key::LeftShift)] | m_isKeyDown[static_cast<size_t>(Key::RightShift)];
        break;

    case Key::LeftControl:
    case Key::RightControl:
        m_isKeyDown[static_cast<size_t>(Key::Control)] = m_isKeyDown[static_cast<size_t>(Key::LeftControl)] | m_isKeyDown[static_cast<size_t>(Key::RightControl)];
        break;

    case Key::LeftAlt:
    case Key::RightAlt:
        m_isKeyDown[static_cast<size_t>(Key::Alt)] = m_isKeyDown[static_cast<size_t>(Key::LeftAlt)] | m_isKeyDown[static_cast<size_t>(Key::RightAlt)];
        break;

    case Key::LeftSuper:
    case Key::RightSuper:
        m_isKeyDown[static_cast<size_t>(Key::Super)] = m_isKeyDown[static_cast<size_t>(Key::LeftSuper)] | m_isKeyDown[static_cast<size_t>(Key::RightSuper)];
        break;

    default:
        break;
    }

}

void WindowInput::OnMouseKeyEvent(Key code, KeyAction action, uint8_t mods) noexcept {
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

void WindowInput::OnCharEvent(char16_t ch) {
    m_userInput += ch;
}
