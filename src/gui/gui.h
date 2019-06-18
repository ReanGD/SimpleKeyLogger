#pragma once

#include "window/window.h"


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
    void EnableInput(bool value);
    void Update(Window& window, float deltaTime);

    void NewFrame() const;
    void EndFrame() const;

private:
    bool m_enableInput = true;
    ImGuiContext* m_context = nullptr;
};
