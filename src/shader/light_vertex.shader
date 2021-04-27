#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 fColor;

void main()
{
	gl_Position = projection * view * model * vec4(vPos, 1.0);
	fColor = vColor;
}