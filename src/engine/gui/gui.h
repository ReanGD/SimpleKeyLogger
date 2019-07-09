#pragma once

#include "engine/window/window.h"


struct ImGuiContext;
class Gui : Noncopyable {
public:
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
