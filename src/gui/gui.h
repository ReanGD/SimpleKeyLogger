#pragma once

#include "window/window_input.h"


struct ImGuiContext;
class Gui {
public:
    Gui(const Gui&) = delete;
    Gui(Gui&&) = delete;
    Gui& operator=(const Gui&) = delete;
    Gui& operator=(Gui&&) = delete;

    Gui() = default;
    ~Gui();

public:
    bool Init(std::string& error);
    void Update(WindowInput& wio, float deltaTime);

private:
    ImGuiContext* m_context = nullptr;
};
