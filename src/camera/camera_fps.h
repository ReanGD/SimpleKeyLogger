#pragma once

#include <memory>
#include <vector>
#include "camera/camera.h"
#include "window/window.h"


class CameraFps : public InputHandler {
public:
    CameraFps() = default;
    ~CameraFps() = default;

public:
    void SetMovementSpeed(float value) noexcept;
    void SetMouseSensitivity(float value) noexcept;
    void AttachCamera(std::shared_ptr<Camera> camera);

    void KeyHandler(const Executor& e) override;
    void MouseHandler(float dtX, float dtY) override;
    void ScreenHandler(uint32_t width, uint32_t height) override;

    void Update(float dt);
private:
    float m_movementSpeed = 2.0f;
    float m_mouseSensitivity = 0.2f;
    float m_aspectRatio = 4.0f / 3.0f;

    // Offset in the current frame
    glm::vec3 m_posOffset = glm::vec3(0);
    // Offset in the previous frame
    glm::vec3 m_posOffsetPrevious = glm::vec3(0);

    // Current camera angle left/right (in the radians)
    float m_yaw = 0;
    // Offset in the current frame
    float m_yawOffset = 0;
    // Offset in the previous frame
    float m_yawOffsetPrevious = 0;

    // Current camera angle up/down (in the radians)
    float m_pitch = 0;
    // Offset in the current frame
    float m_pitchOffset = 0;
    // Offset in the previous frame
    float m_pitchOffsetPrevious = 0;

    std::vector<std::shared_ptr<Camera>> m_cameras;
};
