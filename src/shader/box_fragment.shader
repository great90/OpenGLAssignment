#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D tex;
uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec3 fPos;
in vec3 fNormal;
in vec2 fUV;

out vec4 FragColor;

void main()
{
	// ambient
	vec3 ambient = material.ambient * light.ambient;
	// diffuse
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(light.position - fPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = material.diffuse * diff * light.diffuse;
	// specular
	vec3 viewDir = normalize(viewPos - fPos);
	/*vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);*/
	vec3 mid = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, mid), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular;

	FragColor = texture(tex, fUV) * vec4(ambient + diffuse + specular, 1.0);
}
