#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 fPos;
out vec3 fNormal;
out vec2 fUV;

void main()
{
	fPos = vec3(model * vec4(vPos, 1.0));
	gl_Position = projection * view * vec4(fPos, 1.0);
	fNormal = mat3(transpose(inverse(model))) * vNormal;
	fUV = vUV;
}