#pragma once

#include <memory>
#include <vector>
#include "engine/camera/camera.h"
#include "engine/window/window.h"


// First person camera control
class FPCameraControl {
public:
    enum class Action : uint8_t {
        Forward = 0,
        Backward = 1,
        Right = 2,
        Left = 3,
        Last = 4,
    };
    FPCameraControl() = default;
    ~FPCameraControl() = default;

public:
    void EnableInput(bool value) noexcept;
    void SetHotkey(Action action, Key key) noexcept;
    void SetMovementSpeed(float value) noexcept;
    void SetMouseSensitivity(float value) noexcept;
    void AttachCamera(std::shared_ptr<Camera> camera);

    void Update(WindowInput& wio, float deltaTime);
private:
    bool m_enableInput = true;
    float m_movementSpeed = 2.0f;
    float m_mouseSensitivity = 0.2f;
    float m_aspectRatio = 4.0f / 3.0f;

    // Offset in the previous frame
    glm::vec3 m_posOffsetPrevious = glm::vec3(0);

    // Current camera angle left/right (in the radians)
    float m_yaw = 0;
    // Offset in the previous frame
    float m_yawOffsetPrevious = 0;

    // Current camera angle up/down (in the radians)
    float m_pitch = 0;
    // Offset in the previous frame
    float m_pitchOffsetPrevious = 0;

    Key m_hostkey[static_cast<size_t>(FPCameraControl::Action::Last)] = {
        /*Forward*/Key::W, /*Backward*/Key::S, /*Right*/Key::D, /*Left*/Key::A};
    std::vector<std::shared_ptr<Camera>> m_cameras;
};
