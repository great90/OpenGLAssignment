#include "camera.h"
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <cstdio>

Camera::Camera(float fov, float aspect, float near, float far, const Vector3& pos, const Vector3& forward, const Vector3& up)
	: _fov(fov)
	, _aspect(aspect)
	, _near(near)
	, _far(far)
	, _yaw(-90.0f)
	, _pitch(0.0f)
	, _position(pos)
	, _forward(forward)
	, _up(up)
{
}

void Camera::rotate(float yaw, float pitch, bool constrain_pitch)
{
	_yaw += yaw;
	_pitch += pitch;

	if (constrain_pitch)
	{
		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
	}
	// printf("yaw: +%.4f = %.4f pitch: +%.4f = %.4f\n", yaw, _yaw, pitch, _pitch);

	update_vectors();
}

Matrix4 Camera::get_projection_matrix() const
{
	return glm::perspective(glm::radians(_fov), _aspect, _near, _far);
}

Matrix4 Camera::get_view_matrix() const
{
	return glm::lookAt(_position, _position + glm::normalize(_forward), glm::normalize(_up));
}

void Camera::update_vectors()
{
	Vector3 forward;
	forward.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	forward.y = sin(glm::radians(_pitch));
	forward.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_forward = glm::normalize(forward);
	const Vector3 right = glm::normalize(glm::cross(_forward, Vector3(0.0f, 1.0f, 0.0f)));
	_up = glm::normalize(glm::cross(right, _forward));
}
