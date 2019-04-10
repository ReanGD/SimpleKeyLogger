#pragma once

#include <memory>
#include <vector>
#include "camera/camera.h"


class CameraFps {
public:
	CameraFps() = default;
	~CameraFps() = default;

public:
	void SetMovementSpeed(float value) noexcept;
	void SetMouseSensitivity(float value) noexcept;

	void SetPosition(const glm::vec3& pos);
	// In the radians
	void SetTurn(float yaw, float pitch) noexcept;

	void MoveForward() noexcept;
	void MoveBackward() noexcept;
	void MoveLeft() noexcept;
	void MoveRight() noexcept;
	// mouse move: yaw - left/right, pitch - up/down
	void Rotate(float yaw, float pitch) noexcept;

	void Update(float dt);

	void AttachCamera(std::shared_ptr<Camera> camera);	
private:
	float m_movementSpeed = 2.0f;
	float m_mouseSensitivity = 0.2f;
	float m_pitchMax = glm::half_pi<float>() - 0.2f;
	float m_pitchMin = -(glm::half_pi<float>() - 0.2f);

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
