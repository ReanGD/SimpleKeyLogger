#pragma once

#include "engine/engine.h"


struct ImFont;
class UIInterface : Noncopyable {
public:
    UIInterface(const UIInterface&) = delete;
    UIInterface(UIInterface&&) = delete;
    UIInterface& operator=(const UIInterface&) = delete;
    UIInterface& operator=(UIInterface&&) = delete;

    UIInterface() = default;
    ~UIInterface() = default;

    bool Init(const Gui& gui, std::string& error);
    void Draw(Engine& engine, bool editorMode);
private:
    void DrawInfoBar(Engine& engine);
    void DrawRightPanel(Engine& engine);
    void DrawExample();
private:
    ImFont* m_fontDefault = nullptr;
    ImFont* m_fontMono = nullptr;
};
