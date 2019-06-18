#pragma once

#include "gui/gui.h"


class UIInterface {
public:
    UIInterface(const UIInterface&) = delete;
    UIInterface(UIInterface&&) = delete;
    UIInterface& operator=(const UIInterface&) = delete;
    UIInterface& operator=(UIInterface&&) = delete;

    UIInterface() = default;
    ~UIInterface();

    void Draw(const Gui& gui);

private:
    bool m_showDemoWindow = false;
    bool m_showAnotherWindow = false;
};
