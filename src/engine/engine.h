#pragma once

#include <functional>

#include "engine/gui/gui.h"


class Engine {
public:
    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&) = delete;

    Engine() = default;
    ~Engine() = default;

    bool Init(bool isFullscreen, float windowMultiplier, std::string& error);
    void Run(const std::function<void (Engine&)>& callback);

    Gui& GetGui() noexcept {
        return m_gui;
    }

    Window& GetWindow() noexcept {
        return m_window;
    }

    float GetDeltaTime() const noexcept {
        return m_deltaTime;
    }
private:
    Gui m_gui;
    Window m_window;
    float m_deltaTime = 1.0f / 60.0f;
};
