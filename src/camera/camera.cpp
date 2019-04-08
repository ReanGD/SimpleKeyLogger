#include "camera/camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() {
	recalcProjMatrix();
	recalcViewMatrix();
}

Camera::Camera(float fovy, uint width, uint height, float nearPlane, float farPlane)
	: m_fovy(fovy)
	, m_aspectRatio(float(width) / float(height))
	, m_nearPlane(nearPlane)
	, m_farPlane(farPlane) {
	recalcProjMatrix();
	recalcViewMatrix();
}

void Camera::recalcProjMatrix() {
	m_matProj = glm::perspective(m_fovy, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::recalcViewMatrix(void) {
	m_direction = glm::normalize(m_direction);
	glm::vec3 at(m_from + m_direction);
	m_matView = glm::lookAt(m_from, at, m_up);
}

void Camera::SetProjParams(float fovy, float aspectRatio, float nearPlane, float farPlane) {
	m_nearPlane		= nearPlane;
	m_farPlane		= farPlane;
	m_fovy			= fovy;
	m_aspectRatio	= aspectRatio;
	recalcProjMatrix();
}

void Camera::SetPosition(const glm::vec3& position) {
	m_from = position;
	recalcViewMatrix();
}

void Camera::SetDirection(const glm::vec3& direction) {
	m_direction = direction;
	recalcViewMatrix();
}

void Camera::SetViewParams(const glm::vec3& position, const glm::vec3& direction) {
	m_from = position;
	m_direction	= direction;
	recalcViewMatrix();
}

glm::vec3 Camera::GetPosition() const {
	return m_from;
}

glm::vec3 Camera::GetDirection() const {
	return m_direction;
}

glm::vec3 Camera::GetCrossVector() const {
	return glm::normalize(glm::cross(m_direction, m_up));
}

glm::mat4 Camera::GetProjMatrix(void) const {
	return m_matProj;
}

glm::mat4 Camera::GetViewMatrix(void) const {
	return m_matView;
}
