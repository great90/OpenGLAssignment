#version 330 core

struct Material {
	sampler2D diffuse_textures[1];
};

uniform Material material;

in vec2 fUV;
out vec4 FragColor;

void main()
{
	FragColor = texture(material.diffuse_textures[0], fUV);
}
