#pragma once

#include <memory>
#include <vector>
#include "engine/camera/camera.h"
#include "engine/window/window.h"


// First-person flight camera controler
class FlyCameraController : Noncopyable {
public:
    enum class Action : uint8_t {
        Forward = 0,
        Backward = 1,
        Right = 2,
        Left = 3,
        LowSpeed = 4,
        HighSpeed = 5,
        Last = 6,
    };
    FlyCameraController() = default;
    ~FlyCameraController() = default;

public:
    void EnableInput(bool value) noexcept;
    void SetHotkey(Action action, Key key) noexcept;
    void SetMementSmoothFactor(float value) noexcept;
    void SetMovementSpeed(float low, float normal, float high) noexcept;
    void SetMouseSmooth(float value) noexcept;
    void SetMouseSensitivity(float value) noexcept;
    void AttachCamera(const std::shared_ptr<Camera>& camera);

    void Update(WindowInput& wio, float deltaTime);
private:
    bool m_enableInput = true;
    float m_movementSmoothFactor = 0.5f;
    float m_movementLowSpeed = 2.0f;
    float m_movementNormalSpeed = 5.0f;
    float m_movementHighSpeed = 15.0f;
    float m_mouseSmoothFactor = 0.5f;
    float m_mouseSensitivity = 10.0f;
    float m_aspectRatio = 4.0f / 3.0f;

    glm::vec3 m_movementSpeed = glm::vec3(0);

    // Current camera angle in the radians (x: yaw - left/right,  y: pitch - up/down)
    glm::vec2 m_rotation = glm::vec2(0);
    glm::vec2 m_rotationSpeed = glm::vec2(0);
    glm::vec3 m_direction = glm::vec3(1.0f, 0.0f, 0.0f);

    Key m_hostkey[static_cast<size_t>(FlyCameraController::Action::Last)] = {
        /*Forward*/Key::W, /*Backward*/Key::S, /*Right*/Key::D, /*Left*/Key::A, /*LowSpeed*/Key::Control, /*HighSpeed*/Key::Shift};
    std::vector<std::shared_ptr<Camera>> m_cameras;
};
