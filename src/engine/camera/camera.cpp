#include "engine/camera/camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(float fovy, float nearPlane, float farPlane)
	: m_fovy(fovy)
	, m_nearPlane(nearPlane)
	, m_farPlane(farPlane) {
	m_matProj = glm::perspective(m_fovy, m_aspectRatio, m_nearPlane, m_farPlane);
	calcViewMatrix(glm::vec3(1.0, 0.0, 0.0));
}

void Camera::SetAspectRatio(float value) {
	m_aspectRatio = value;
	m_matProj = glm::perspective(m_fovy, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::SetNearPlane(float value) {
	m_nearPlane = value;
	m_matProj = glm::perspective(m_fovy, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::SetFarPlane(float value) {
	m_farPlane = value;
	m_matProj = glm::perspective(m_fovy, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::SetViewParams(const glm::vec3& position, const glm::vec3& direction) {
	m_position = position;
	calcViewMatrix(direction);
}

glm::vec3 Camera::HomogeneousPositionToRay(const glm::vec2& pos) const noexcept {
	// see: http://antongerdelan.net/opengl/raycasting.html
	glm::vec4 rayEye = glm::inverse(m_matProj) * glm::vec4(pos.x, pos.y, -1.0f, 1.0f);
	rayEye.z = -1.0f;
	rayEye.w = 0.0f;

	return glm::normalize(glm::vec3(glm::inverse(m_matView) * rayEye));
}

void Camera::calcViewMatrix(const glm::vec3& dir) {
	// see glm::lookAtRH
	constexpr const glm::vec3 up(0.0, 1.0, 0.0);
	glm::vec3 const direction(glm::normalize(dir));
	glm::vec3 const lvec(glm::normalize(glm::cross(direction, up)));
	glm::vec3 const u(glm::cross(lvec, direction));

	m_matView[0][0] = lvec.x;
	m_matView[1][0] = lvec.y;
	m_matView[2][0] = lvec.z;

	m_matView[0][1] = u.x;
	m_matView[1][1] = u.y;
	m_matView[2][1] = u.z;

	m_matView[0][2] =-direction.x;
	m_matView[1][2] =-direction.y;
	m_matView[2][2] =-direction.z;

	m_matView[3][0] =-glm::dot(lvec, m_position);
	m_matView[3][1] =-glm::dot(u, m_position);
	m_matView[3][2] = glm::dot(direction, m_position);
}
