#pragma once

#include <functional>

#include "engine/gui/gui.h"
#include "engine/physics/physics.h"


class Engine : Noncopyable {
public:
    Engine() = default;
    ~Engine() = default;

    void Create(bool isFullscreen, float windowMultiplier);
    void Run(const std::function<void (float /* deltaTime */)>& updateCallback, const std::function<void ()>& drawCallback);

    Gui& GetGui() noexcept {
        return m_gui;
    }

    Window& GetWindow() noexcept {
        return m_window;
    }

    Physics& GetPhysics() noexcept {
        return m_physics;
    }

    void SetFillPoligone(bool value) noexcept;

    bool IsFillPoligone() const noexcept {
        return m_fillPoligone;
    }

    float GetFps() const noexcept {
        return static_cast<float>(m_timeDeltas.size()) / m_timeDeltasTotal;
    }

private:
    Gui m_gui;
    Window m_window;
    Physics m_physics;
    bool m_fillPoligone = true;
    float m_deltaTime = 1.0f / 60.0f;
    uint16_t m_timeDeltasPos = 0;
    std::array<float, 120> m_timeDeltas = { /*1.0f / 60.0f*/ };
    float m_timeDeltasTotal = static_cast<float>(m_timeDeltas.size()) / 60.0f;
};
