#pragma once

#include "engine/window/window.h"


class Texture;
struct ImGuiContext;
class Gui : Noncopyable {
public:
    Gui() = default;
    ~Gui();

public:
    void Create();
    void EnableInput(bool value);
    void Update(Window& window, float deltaTime);

    void NewFrame() const;
    void EndFrame() const;
private:
    bool m_enableInput = true;
    ImGuiContext* m_context = nullptr;
};
