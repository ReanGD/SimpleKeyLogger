#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/constants.hpp>


class Camera {
public:
	Camera();
	Camera(float fovy, float nearPlane, float farPlane);
	~Camera() = default;

public:
	void SetAspectRatio(float value);

	void SetPosition(const glm::vec3& value);
	void SetDirection(const glm::vec3& value);
	void SetViewParams(const glm::vec3& position, const glm::vec3& direction);

	glm::vec3 GetPosition() const noexcept {
		return m_from;
	}
	glm::vec3 GetDirection() const noexcept {
		return m_direction;
	}

	// A normalized vector that is directed to the left of the direction of the gaze
	glm::vec3 GetLeftVector() const;

	glm::mat4 GetProjMatrix() const noexcept {
		return m_matProj;
	}
	glm::mat4 GetViewMatrix() const noexcept {
		return m_matView;
	}

private:
	void recalcProjMatrix();
	void recalcViewMatrix();

private:
	glm::vec3 m_from = glm::vec3(0.0);
	// Always normalized
	glm::vec3 m_direction = glm::vec3(1.0, 0.0, 0.0);
	// Y-axis up
	glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);
	// In the radians (45 degrees)
	float m_fovy = glm::quarter_pi<float>();
	// screenWidth / screenHeight
	float m_aspectRatio = 4.0f / 3.0f;
	float m_nearPlane = 0.1f;
	float m_farPlane = 100.0;

	glm::mat4 m_matProj;
	glm::mat4 m_matView;
};
