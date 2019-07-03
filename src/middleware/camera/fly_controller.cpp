#include "middleware/camera/fly_controller.h"

#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>


void FlyCameraController::EnableInput(bool value) noexcept {
    m_enableInput = value;
}

void FlyCameraController::SetHotkey(Action action, Key key) noexcept {
    if (action < Action::Last) {
        m_hostkey[static_cast<size_t>(action)] = key;
    }
}

void FlyCameraController::SetMementSmoothFactor(float value) noexcept {
    m_movementSmoothFactor = value;
}

void FlyCameraController::SetMovementNormalSpeed(float value) noexcept {
    m_movementNormalSpeed = value;
}

void FlyCameraController::SetMouseSmooth(float value) noexcept {
    m_mouseSmoothFactor = value;
}

void FlyCameraController::SetMouseSensitivity(float value) noexcept {
    m_mouseSensitivity = value;
}

void FlyCameraController::AttachCamera(std::shared_ptr<Camera> camera) {
    camera->SetAspectRatio(m_aspectRatio);
    camera->SetViewParams(camera->GetPosition(), m_direction);
    m_cameras.push_back(camera);
}

void FlyCameraController::Update(WindowInput& wio, float deltaTime) {
    constexpr const auto mouseSensitivityFactor = 1.0f / 2000.0f;
    constexpr const auto pitchMax = glm::half_pi<float>() - 0.2f;
    constexpr const auto pitchMin = -(glm::half_pi<float>() - 0.2f);
    constexpr const auto matOne = glm::mat4(1.0);
    constexpr const auto yawAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr const auto pitchAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    constexpr const auto vecEye = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 positionOffset = glm::vec3(0);
    if (m_enableInput) {
        // xxxSmoothFactor - for 60 FPS, recalc for our FPS:
        float smoothFPSCorrection = deltaTime * 60;

        bool isMove = false;
        glm::vec3 moveDirection(0);
        if (wio.IsKeyStickyDown(m_hostkey[static_cast<size_t>(Action::Forward)])) {
            moveDirection.x += 1.0f;
            isMove = true;
        } else if (wio.IsKeyStickyDown(m_hostkey[static_cast<size_t>(Action::Backward)])) {
            moveDirection.x -= 1.0f;
            isMove = true;
        }
        if (wio.IsKeyStickyDown(m_hostkey[static_cast<size_t>(Action::Right)])) {
            moveDirection.y += 1.0f;
            isMove = true;
        } else if (wio.IsKeyStickyDown(m_hostkey[static_cast<size_t>(Action::Left)])) {
            moveDirection.y -= 1.0f;
            isMove = true;
        }
        if (isMove) {
            auto currentSpeed = glm::normalize(moveDirection) * m_movementNormalSpeed;
            m_movementSpeed = glm::lerp(currentSpeed, m_movementSpeed, glm::min(m_movementSmoothFactor * smoothFPSCorrection, 0.9f));
            positionOffset = (m_movementSpeed * deltaTime);
        }

        bool isRotate = false;
        glm::vec2 dtRotation;
        wio.GetCursorOffet(dtRotation.x, dtRotation.y);
        if (glm::abs(dtRotation.x) > 0.1f || glm::abs(dtRotation.y) > 0.1f) {
            isRotate = true;
            auto currentRotationSpeed = dtRotation / deltaTime;
            m_rotationSpeed = glm::lerp(currentRotationSpeed, m_rotationSpeed, glm::min(m_mouseSmoothFactor * smoothFPSCorrection, 0.9f));
            m_rotation += (m_rotationSpeed * m_mouseSensitivity * mouseSensitivityFactor * deltaTime);

            m_rotation.x = std::fmod(m_rotation.x, glm::two_pi<float>());
            m_rotation.y = glm::clamp(m_rotation.y, pitchMin, pitchMax);
            m_direction = glm::rotate(matOne, m_rotation.x, yawAxis) * glm::rotate(matOne, m_rotation.y, pitchAxis) * vecEye;
        }

        if (isMove || isRotate) {
            for(const auto& camera: m_cameras) {
                auto position =
                    camera->GetPosition() +
                    camera->GetDirection() * positionOffset.x +
                    camera->GetLeftVector() * positionOffset.y;
                camera->SetViewParams(position, m_direction);
            }
        }
    }

    uint32_t width, height;
    if (wio.GetFramebufferSize(width, height)) {
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        for(const auto& camera: m_cameras) {
            camera->SetAspectRatio(m_aspectRatio);
        }
    }
}
