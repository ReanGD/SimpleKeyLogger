#pragma once

#include "engine/gui/gui.h"


class UIInterface {
public:
    UIInterface(const UIInterface&) = delete;
    UIInterface(UIInterface&&) = delete;
    UIInterface& operator=(const UIInterface&) = delete;
    UIInterface& operator=(UIInterface&&) = delete;

    UIInterface() = default;
    ~UIInterface() = default;

    void Draw(const Gui& gui);

private:
    bool m_showDemoWindow = false;
    bool m_showAnotherWindow = false;
};
