#pragma once

#include <functional>

#include "engine/gui/gui.h"
#include "engine/scene/scene.h"


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

    Scene& GetScene() noexcept {
        return m_scene;
    }

    float GetDeltaTime() const noexcept {
        return m_deltaTime;
    }

    float GetFps() const noexcept {
        return static_cast<float>(m_timeDeltas.size()) / m_timeDeltasTotal;
    }
private:
    Gui m_gui;
    Window m_window;
    Scene m_scene;
    float m_deltaTime = 1.0f / 60.0f;
    uint16_t m_timeDeltasPos = 0;
    std::array<float, 120> m_timeDeltas = { /*1.0f / 60.0f*/ };
    float m_timeDeltasTotal = static_cast<float>(m_timeDeltas.size()) / 60.0f;
};
