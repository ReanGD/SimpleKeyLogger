#pragma once

#include <string>


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

private:
    ImGuiContext* m_context = nullptr;
};
