#include "engine/engine.h"

#include <chrono>
#include "engine/api/gl.h"


bool Engine::Init(bool isFullscreen, float windowMultiplier, std::string& error) {
    m_timeDeltas.fill(1.0f / 60.0f);
    if (!m_window.Init(isFullscreen, windowMultiplier, error)) {
        return false;
    }

    if (!GLApi::Init(error)) {
        return false;
    }

    if (!m_gui.Init(error)) {
        return false;
    }

    if (!m_physics.Init(error)) {
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glLineWidth(2.0f);
    SetFillPoligone(m_fillPoligone);

    return true;
}

void Engine::Run(const std::function<void ()>& callback) {
    auto timeLast = std::chrono::steady_clock::now();

    uint32_t width, height;
    auto& wio = m_window.GetIO();

    while (m_window.StartFrame()) {
        wio.GetFramebufferSize(width, height);
        glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        auto now = std::chrono::steady_clock::now();
        m_deltaTime = std::chrono::duration<float>(now - timeLast).count();
        timeLast = now;

        m_timeDeltasTotal += (m_deltaTime - m_timeDeltas[m_timeDeltasPos]);
        m_timeDeltas[m_timeDeltasPos] = m_deltaTime;
        m_timeDeltasPos = (m_timeDeltasPos+1) % m_timeDeltas.size();

        m_gui.Update(m_window, m_deltaTime);
        m_physics.Update(m_deltaTime);
        m_scene.Update();
        callback();

        m_window.EndFrame();
    }
}

void Engine::SetFillPoligone(bool value) noexcept {
    m_fillPoligone = value;
    glPolygonMode(GL_FRONT_AND_BACK, m_fillPoligone ? GL_FILL : GL_LINE);
}
