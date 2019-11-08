#pragma once

#include <functional>

#include "engine/gui/gui.h"
#include "engine/scene/scene.h"
#include "engine/physics/physics.h"


class Engine : Noncopyable {
public:
    Engine() = default;
    ~Engine() = default;

    void Init(bool isFullscreen, float windowMultiplier);
    void Run(const std::function<void ()>& callback);

    Gui& GetGui() noexcept {
        return m_gui;
    }

    Window& GetWindow() noexcept {
        return m_window;
    }

    Scene& GetScene() noexcept {
        return m_scene;
    }

    Physics& GetPhysics() noexcept {
        return m_physics;
    }

    void SetFillPoligone(bool value) noexcept;

    bool IsFillPoligone() const noexcept {
        return m_fillPoligone;
    }

    float GetDeltaTime() const noexcept {
        return m_deltaTime;
    }

    float GetFps() const noexcept {
        return static_cast<float>(m_timeDeltas.size()) / m_timeDeltasTotal;
    }

    uint32_t GetTpf() const noexcept {
        return m_scene.GetCountTriangles();
    }
private:
    Gui m_gui;
    Scene m_scene;
    Window m_window;
    Physics m_physics;
    bool m_fillPoligone = true;
    float m_deltaTime = 1.0f / 60.0f;
    uint16_t m_timeDeltasPos = 0;
    std::array<float, 120> m_timeDeltas = { /*1.0f / 60.0f*/ };
    float m_timeDeltasTotal = static_cast<float>(m_timeDeltas.size()) / 60.0f;
};
