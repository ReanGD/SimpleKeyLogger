#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/constants.hpp>
#include "engine/common/noncopyable.h"


class Camera : Noncopyable {
public:
	Camera() = delete;
	Camera(float fovy, float nearPlane, float farPlane);
	~Camera() = default;

public:
	// screenWidth / screenHeight
	void SetAspectRatio(float value);
	void SetViewParams(const glm::vec3& position, const glm::vec3& direction);

	glm::vec3 GetPosition() const noexcept {
		return m_position;
	}

	glm::vec3 GetDirection() const noexcept {
		return glm::vec3(-m_matView[0][2], -m_matView[1][2], -m_matView[2][2]);
	}

	glm::vec3 GetToEyeDirection() const noexcept {
		return glm::vec3(m_matView[0][2], m_matView[1][2], m_matView[2][2]);
	}

	// A normalized vector that is directed to the left of the direction of the gaze
	glm::vec3 GetLeftVector() const noexcept {
		return glm::vec3(m_matView[0][0], m_matView[1][0], m_matView[2][0]);
	}

	glm::mat4 GetProjMatrix() const noexcept {
		return m_matProj;
	}

	glm::mat4 GetViewMatrix() const noexcept {
		return m_matView;
	}

private:
	void calcViewMatrix(const glm::vec3& direction);

private:
	glm::vec3 m_position = glm::vec3(0.0);
	// In the radians (45 degrees)
	float m_fovy = glm::quarter_pi<float>();
	// screenWidth / screenHeight
	float m_aspectRatio = 4.0f / 3.0f;
	float m_nearPlane = 0.1f;
	float m_farPlane = 100.0;

	glm::mat4 m_matProj;
	glm::mat4 m_matView = glm::mat4(1);
};
