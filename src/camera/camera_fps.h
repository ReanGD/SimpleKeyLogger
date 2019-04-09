#pragma once

#include <memory>
#include <vector>
#include "camera/camera.h"


class CameraFps {
public:
	CameraFps() = default;
	~CameraFps() = default;

public:
    void SetMovementSpeed(float value);
    void SetMouseSensitivity(float value);

	void SetPosition(const glm::vec3& pos);
    // In the radians
	void SetTurn(float yaw, float pitch);

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
    // mouse left/right
	void RotateYaw(float value);
	// mouse up/down
	void RotatePitch(float value);

    void Update(float dt);

    void AttachCamera(std::shared_ptr<Camera> camera);	
private:
    float m_movementSpeed = 1.0f;
    float m_mouseSensitivity = 1.0f;
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
