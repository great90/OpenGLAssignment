#pragma once

#include "math/math.h"

class Camera
{
public:
	Camera(float fov, float aspect, float near, float far, const Vector3& pos, const Vector3& forward, const Vector3& up = Vector3(0.0f, 1.0f, 0.0f));
	~Camera() = default;

	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) = delete;

	void move(const Vector3& displacement) { _position += displacement; }
	void rotate(float yaw, float pitch, bool constrain_pitch = true);

	Matrix4 get_projection_matrix() const;
	Matrix4 get_view_matrix() const;

	float get_fov() const { return _fov; }
	void set_fov(float fov) { _fov = glm::clamp(fov, 10.0f, 80.0f); }
	float get_aspect() const { return _aspect; }
	void set_aspect(float aspect) { _aspect = aspect; }
	float get_near() const { return _near; }
	void set_near(float near) { _near = near; }
	float get_far() const { return _far; }
	void set_far(float far) { _far = far; }

	float get_yaw() const { return _yaw; }
	void set_yaw(float yaw) { _yaw = yaw; }
	float get_pitch() const { return _pitch; }
	void set_pitch(float pitch) { _pitch = pitch; }

	const Vector3& get_position() const { return _position; }
	void set_position(const Vector3& pos) { _position = pos; }
	const Vector3& get_forward() const { return _forward; }
	void set_forward(const Vector3& forward) { _forward = forward; }
	const Vector3& get_up() const { return _up; }
	void set_up(const Vector3& up) { _up = up; }

protected:
	void update_vectors();
	
private:
	float _fov;
	float _aspect;
	float _near;
	float _far;
	float _yaw;
	float _pitch;
	Vector3 _position;
	Vector3 _forward;
	Vector3 _up;
};
