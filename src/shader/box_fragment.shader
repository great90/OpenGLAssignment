#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

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
	vec3 ambient = texture(material.diffuse, fUV).rgb * light.ambient;
	// diffuse
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(light.position - fPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = texture(material.diffuse, fUV).rgb * diff * light.diffuse;
	// specular
	vec3 viewDir = normalize(viewPos - fPos);
	/*vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);*/
	vec3 mid = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, mid), 0.0), material.shininess);
	vec3 specular = texture(material.specular, fUV).rgb * spec * light.specular;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
