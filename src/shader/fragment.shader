#version 330 core

uniform sampler2D tex;

in vec3 fColor;
in vec2 fUV;

out vec4 FragColor;

void main()
{
	FragColor = texture(tex, fUV) * vec4(fColor, 1.0f);
}
