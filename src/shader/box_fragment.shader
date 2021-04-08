#version 330 core

uniform sampler2D tex;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 fPos;
in vec3 fNormal;
in vec2 fUV;

out vec4 FragColor;

void main()
{
	// ambient
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;
	// diffuse
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	float specularStrength = 0.99;
	vec3 viewDir = normalize(viewPos - fPos);
	/*vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);*/
	vec3 mid = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, mid), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	FragColor = texture(tex, fUV) * vec4(objectColor * (ambient + diffuse + specular), 1.0);
}
