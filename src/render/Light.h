#pragma once

#include "math/math.h"
#include "shader.h"

enum class LightType : unsigned int
{
	Directional,
	Omni,
	Spot
};

struct Light
{
	LightType type;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	union
	{
		struct
		{
			Vector3 direction;
		} directional;

		struct 
		{
			Vector3 position;
			float constant;
			float linear;
			float quadratic;
		} omni;

		struct 
		{
			Vector3 position;
			Vector3 direction;
			float constant;
			float linear;
			float quadratic;

			float innerCutOff;
			float outerCutOff;
		} spot;
	};

	void bind(ShaderProgram& shader, const std::string& name) const
	{
		switch (type)
		{
		case LightType::Directional:
			shader.set_vector3(name + ".direction", directional.direction);
			break;
		case LightType::Omni:
			shader.set_vector3(name + ".position", omni.position);
			shader.set_float(name + ".constant", omni.constant);
			shader.set_float(name + ".linear", omni.linear);
			shader.set_float(name + ".quadratic", omni.quadratic);
			break;
		case LightType::Spot:
			shader.set_vector3(name + ".position", spot.position);
			shader.set_vector3(name + ".direction", spot.direction);
			shader.set_float(name + ".constant", spot.constant);
			shader.set_float(name + ".linear", spot.linear);
			shader.set_float(name + ".quadratic", spot.quadratic);
			shader.set_float(name + ".innerCutOff", spot.innerCutOff);
			shader.set_float(name + ".outerCutOff", spot.outerCutOff);
			break;
		default:
			assert(0);
			break;
		}
		shader.set_vector3(name + ".ambient", ambient);
		shader.set_vector3(name + ".diffuse", diffuse);
		shader.set_vector3(name + ".specular", specular);
	}
};
