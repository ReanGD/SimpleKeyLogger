#include "camera/camera_fps.h"
#include <glm/gtc/matrix_transform.hpp>


void CameraFps::SetMovementSpeed(float value) noexcept {
    m_movementSpeed	= value;
}

void CameraFps::SetMouseSensitivity(float value) noexcept {
    m_mouseSensitivity = value;
}

void CameraFps::SetPosition(const glm::vec3& value) {
    for(const auto& camera: m_cameras) {
        camera->SetPosition(value);
    }
}

void CameraFps::SetTurn(float yaw, float pitch) noexcept {
    m_yaw = yaw;
    m_pitch	= pitch;
}

void CameraFps::MoveForward() noexcept {
    m_posOffset.x += 1.0f;
}

void CameraFps::MoveBackward() noexcept {
    m_posOffset.x -= 1.0f;
}

void CameraFps::MoveLeft() noexcept {
    m_posOffset.y -= 1.0f;
}

void CameraFps::MoveRight() noexcept {
    m_posOffset.y += 1.0f;
}

void CameraFps::Rotate(float dtYaw, float dtPitch) noexcept {
    m_yawOffset += dtYaw;
    m_pitchOffset += dtPitch;
}

void CameraFps::AttachCamera(std::shared_ptr<Camera> camera) {
    m_cameras.push_back(camera);
}

void CameraFps::Update(float dt) {
    const float factor = 0.3f;

    dt *= (1.0f - factor);

    m_posOffsetPrevious *= factor;
    m_yawOffsetPrevious *= factor;
    m_pitchOffsetPrevious *= factor;

    if ((-0.1f > m_posOffset.x) || (m_posOffset.x > 0.1f) ||
        (-0.1f > m_posOffset.y) || (m_posOffset.y > 0.1f) ||
        (-0.1f > m_posOffset.z) || (m_posOffset.z > 0.1f)) {
        m_posOffsetPrevious += (glm::normalize(m_posOffset) * (m_movementSpeed * dt));
        m_posOffset = glm::vec3(0);
    }

    m_yawOffsetPrevious += (m_yawOffset * m_mouseSensitivity * dt);
    m_yawOffset	= 0;
    m_yaw += m_yawOffsetPrevious;
    if (m_yaw >= glm::two_pi<float>()) {
        m_yaw -= glm::two_pi<float>();
    } else if (m_yaw <= -glm::two_pi<float>()) {
        m_yaw += glm::two_pi<float>();
    }

    m_pitchOffsetPrevious += (m_pitchOffset * m_mouseSensitivity * dt);
    m_pitchOffset = 0;
    m_pitch	= glm::min(glm::max(m_pitch + m_pitchOffsetPrevious, m_pitchMin), m_pitchMax);

    const auto matOne = glm::mat4(1.0);
    const auto yawAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    const auto pitchAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    const auto vecEye = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 direction = glm::rotate(matOne, m_yaw, yawAxis) * glm::rotate(matOne, m_pitch, pitchAxis) * vecEye;

    for(const auto& camera: m_cameras) {
        auto position =
            camera->GetPosition() +
            camera->GetDirection() * m_posOffsetPrevious.x +
            camera->GetLeftVector() * m_posOffsetPrevious.y;
        camera->SetViewParams(position, direction);
    }
}
