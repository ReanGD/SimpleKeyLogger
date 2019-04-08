#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/constants.hpp>

class Camera {
public:
	Camera();
	Camera(float fovy, uint width, uint height, float nearPlane, float farPlane);
	~Camera() = default;
protected:
	void recalcProjMatrix();
	void recalcViewMatrix();
public:
	void SetProjParams(float fovy, float aspectRatio, float nearPlane, float farPlane);

	void SetPosition(const glm::vec3& position);
	void SetDirection(const glm::vec3& direction);
	void SetViewParams(const glm::vec3& position, const glm::vec3& direction);

	glm::vec3 GetPosition() const;
	glm::vec3 GetDirection() const;
	// Get a normalized third axis of the camera
	glm::vec3 GetCrossVector() const;

	glm::mat4 GetProjMatrix() const;
	glm::mat4 GetViewMatrix() const;
protected:
	glm::vec3 m_from = glm::vec3(0.0);
	// Always normalized
	glm::vec3 m_direction = glm::vec3(1.0, 0.0, 0.0);
	// Y-axis up
	glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);
	// In the radians (45 degrees)
	float m_fovy = glm::quarter_pi<float>();
	// screenWidth / screenHeight
	float m_aspectRatio = 3.0/4.0;
	float m_nearPlane = 0.1f;
	float m_farPlane = 100.0;	
	
	glm::mat4 m_matProj;
	glm::mat4 m_matView;
};
