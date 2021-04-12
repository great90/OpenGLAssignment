#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 direction;
};

struct OmniLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	vec3 direction;

	float constant;
	float linear;
	float quadratic;

	float innerCutOff;
	float outerCutOff;
};

#define MAX_OMNI_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

uniform Material material;
uniform DirectionalLight directional_light;
uniform OmniLight omni_lights[MAX_OMNI_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];
uniform int omni_light_count;
uniform int spot_light_count;
uniform vec3 viewPos;

in vec3 fPos;
in vec3 fNormal;
in vec2 fUV;

out vec4 FragColor;

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calc_omni_light(OmniLight light, vec3 normal, vec3 fPos, vec3 viewDir);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 fPos, vec3 viewDir);

void main()
{
	vec3 normal = normalize(fNormal);
	vec3 viewDir = normalize(viewPos - fPos);

	vec3 color = calc_directional_light(directional_light, normal, viewDir);
	for (int i = 0; i < omni_light_count; ++i)
		color += calc_omni_light(omni_lights[i], normal, fPos, viewDir);
	for (int i = 0; i < spot_light_count; ++i)
		color += calc_spot_light(spot_lights[i], normal, fPos, viewDir);

	FragColor = vec4(color, 1.0);
}

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, fUV).rgb;
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, fUV).rgb;
	// specular
	/*vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);*/
	vec3 mid = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, mid), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, fUV).rgb;

	return ambient + diffuse + specular;
}

vec3 calc_omni_light(OmniLight light, vec3 normal, vec3 fPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fPos);
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, fUV).rgb;
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, fUV).rgb;
	// specular
	vec3 mid = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, mid), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, fUV).rgb;
	// attenuation
	float distance = length(light.position - fPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	return (ambient + diffuse + specular) * attenuation;
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 fPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fPos);
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, fUV).rgb;
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, fUV).rgb;
	// specular
	vec3 mid = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, mid), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, fUV).rgb;
	// attenuation
	float distance = length(light.position - fPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return (ambient + diffuse + specular) * attenuation * intensity;
}
