#include "engine/camera/fp_camera_control.h"

#include <glm/gtc/matrix_transform.hpp>


void FPCameraControl::EnableInput(bool value) noexcept {
    m_enableInput = value;
}

void FPCameraControl::SetHotkey(Action action, Key key) noexcept {
    if (action < Action::Last) {
        m_hostkey[static_cast<size_t>(action)] = key;
    }
}

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

void FPCameraControl::Update(WindowInput& wio, float deltaTime) {
    constexpr const float factor = 0.3f;
    constexpr const float pitchMax = glm::half_pi<float>() - 0.2f;
    constexpr const float pitchMin = -(glm::half_pi<float>() - 0.2f);

    float dt = deltaTime * (1.0f - factor);

    m_posOffsetPrevious *= factor;
    m_yawOffsetPrevious *= factor;
    m_pitchOffsetPrevious *= factor;

    if (m_enableInput) {
        bool isMove = false;
        glm::vec3 posOffset(0);
        if (wio.IsKeyStickyDown(m_hostkey[static_cast<size_t>(Action::Forward)])) {
            posOffset.x += 1.0f;
            isMove = true;
        } else if (wio.IsKeyStickyDown(m_hostkey[static_cast<size_t>(Action::Backward)])) {
            posOffset.x -= 1.0f;
            isMove = true;
        }
        if (wio.IsKeyStickyDown(m_hostkey[static_cast<size_t>(Action::Right)])) {
            posOffset.y += 1.0f;
            isMove = true;
        } else if (wio.IsKeyStickyDown(m_hostkey[static_cast<size_t>(Action::Left)])) {
            posOffset.y -= 1.0f;
            isMove = true;
        }
        if (isMove) {
            m_posOffsetPrevious += (glm::normalize(posOffset) * (m_movementSpeed * dt));
        }

        float dtYaw, dtPitch;
        wio.GetCursorOffet(dtYaw, dtPitch);
        m_yawOffsetPrevious += (dtYaw * m_mouseSensitivity * dt);
        m_pitchOffsetPrevious += (dtPitch * m_mouseSensitivity * dt);
    }

    m_yaw += m_yawOffsetPrevious;
    if (m_yaw >= glm::two_pi<float>()) {
        m_yaw -= glm::two_pi<float>();
    } else if (m_yaw <= -glm::two_pi<float>()) {
        m_yaw += glm::two_pi<float>();
    }

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

    uint32_t width, height;
    if (wio.GetFramebufferSize(width, height)) {
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        for(const auto& camera: m_cameras) {
            camera->SetAspectRatio(m_aspectRatio);
        }
    }
}
