#include "camera/camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() {
	recalcProjMatrix();
	recalcViewMatrix();
}

Camera::Camera(float fovy, float nearPlane, float farPlane)
	: m_fovy(fovy)
	, m_nearPlane(nearPlane)
	, m_farPlane(farPlane) {
	recalcProjMatrix();
	recalcViewMatrix();
}

void Camera::SetAspectRatio(float value) {
	m_aspectRatio = value;
	recalcProjMatrix();
}

void Camera::SetPosition(const glm::vec3& value) {
	m_from = value;
	recalcViewMatrix();
}

void Camera::SetDirection(const glm::vec3& value) {
	m_direction = value;
	recalcViewMatrix();
}

void Camera::SetViewParams(const glm::vec3& position, const glm::vec3& direction) {
	m_from = position;
	m_direction	= direction;
	recalcViewMatrix();
}

glm::vec3 Camera::GetLeftVector() const {
	return glm::normalize(glm::cross(m_direction, m_up));
}

void Camera::recalcProjMatrix() {
	m_matProj = glm::perspective(m_fovy, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::recalcViewMatrix() {
	m_direction = glm::normalize(m_direction);
	glm::vec3 at(m_from + m_direction);
	m_matView = glm::lookAt(m_from, at, m_up);
}
