#include "window/user_input.h"


static constexpr const uint8_t IsDownMask = 1 << 4;
static constexpr const uint8_t FirstPressMask = 1 << 5;
static constexpr const uint8_t FirstReleaseMask = 1 << 6;


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

bool UserInput::GetKeyDownState(Key code, uint8_t& mods) {
    if (code > Key::Last) {
        return false;
    }

    uint8_t state = m_isKeyDown[static_cast<size_t>(code)];
    mods = state & KeyModifier::Mask;
    return (state & IsDownMask) != 0;
}

std::u16string UserInput::GetInput() {
    return m_userInput;
}

void UserInput::Update() {
    m_userInput.clear();
    for(size_t i=0; i!=static_cast<size_t>(Key::Size); ++i) {
        if ((m_isKeyDown[i] & IsDownMask) != 0) {
            // clear FirstPress and FirstRelease bits
            m_isKeyDown[i] &= ~(FirstPressMask | FirstReleaseMask);
        } else {
            m_isKeyDown[i] = 0;
        }
    }
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
        state |= IsDownMask;
        break;
    case KeyAction::Repeat:
        state |= IsDownMask;
        break;
    }
    m_isKeyDown[static_cast<size_t>(code)] = state;
}

void UserInput::OnKeyEvent(char16_t ch) {
    m_userInput += ch;
}
