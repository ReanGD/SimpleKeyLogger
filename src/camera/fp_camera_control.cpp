#include "camera/fp_camera_control.h"
#include <glm/gtc/matrix_transform.hpp>


void FPCameraControl::SetMovementSpeed(float value) noexcept {
    m_movementSpeed	= value;
}

void FPCameraControl::SetMouseSensitivity(float value) noexcept {
    m_mouseSensitivity = value;
}

void FPCameraControl::AttachCamera(std::shared_ptr<Camera> camera) {
    camera->SetAspectRatio(m_aspectRatio);
    m_cameras.push_back(camera);
}

void FPCameraControl::MoveForward() noexcept {
    m_posOffset.x += 1.0f;
}

void FPCameraControl::MoveBackward() noexcept {
    m_posOffset.x -= 1.0f;
}

void FPCameraControl::MoveLeft() noexcept {
    m_posOffset.y -= 1.0f;
}

void FPCameraControl::MoveRight() noexcept {
    m_posOffset.y += 1.0f;
}

void FPCameraControl::Rotate(float dtYaw, float dtPitch) noexcept {
    m_yawOffset += dtYaw;
    m_pitchOffset += dtPitch;
}

void FPCameraControl::SetScreenSize(uint32_t width, uint32_t height) {
    m_aspectRatio = float(width) / float(height);
    for(const auto& camera: m_cameras) {
        camera->SetAspectRatio(m_aspectRatio);
    }
}

void FPCameraControl::Update(float deltaTime) {
    constexpr const float factor = 0.3f;
    constexpr const float pitchMax = glm::half_pi<float>() - 0.2f;
    constexpr const float pitchMin = -(glm::half_pi<float>() - 0.2f);

    float dt = deltaTime * (1.0f - factor);

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
    m_pitch = glm::min(glm::max(m_pitch + m_pitchOffsetPrevious, pitchMin), pitchMax);

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
