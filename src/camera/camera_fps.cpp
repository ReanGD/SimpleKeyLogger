#include "camera/camera_fps.h"
#include <glm/gtc/matrix_transform.hpp>

void CameraFps::SetMovementSpeed(float value) {
	m_movementSpeed	= value;
}

void CameraFps::SetMouseSensitivity(float value) {
	m_mouseSensitivity = value;
}

void CameraFps::SetPosition(const glm::vec3& value) {
    for(const auto& camera: m_cameras) {
        camera->SetPosition(value);
    }
}

void CameraFps::SetTurn(float yaw, float pitch) {
	m_yaw = yaw;
	m_pitch	= pitch;
}

void CameraFps::MoveForward() {
	m_posOffset.x += 1.0f;
}

void CameraFps::MoveBackward() {
	m_posOffset.x -= 1.0f;
}

void CameraFps::MoveLeft() {
	m_posOffset.y -= 1.0f;
}

void CameraFps::MoveRight() {
	m_posOffset.y += 1.0f;
}

void CameraFps::RotateYaw(float value) {
	m_yawOffset += value;
}

void CameraFps::RotatePitch(float value) {
	m_pitchOffset -= value;
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
	
    glm::vec3 direction =
        glm::rotate(glm::mat4(1.0), m_pitch, glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::rotate(glm::mat4(1.0), m_yaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::vec4(1,0,0,1);

    for(const auto& camera: m_cameras) {
        auto position = camera->GetPosition() + camera->GetDirection() * m_posOffsetPrevious.x + camera->GetCrossVector() * m_posOffsetPrevious.y;
        camera->SetViewParams(position, direction);
    }
}
