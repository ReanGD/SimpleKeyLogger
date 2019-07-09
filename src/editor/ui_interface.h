#pragma once

#include "engine/gui/gui.h"


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
    void Draw(const Gui& gui, float fps);
private:
    void DrawInfoBar(float fps);
    void DrawExample();
private:
    ImFont* m_fontDefault = nullptr;
    ImFont* m_fontMono = nullptr;
};
