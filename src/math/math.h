#pragma once

#include "glm/glm.hpp"

typedef glm::vec2 Vector2;
typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;

typedef glm::mat3x3 Matrix3;
typedef glm::mat4x4 Matrix4;
typedef glm::qua<float> Quaternion;

inline float distance_sqr(const Vector3& lhs, const Vector3& rhs)
{
	return (lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y) + (lhs.z - rhs.z) * (lhs.z - rhs.z);
}

inline float distance(const Vector3& lhs, const Vector3& rhs)
{
	return sqrt(distance_sqr(lhs, rhs));
}
