#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec4 Color;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main(void)
{
	// Ambient
	float ambientStrength = 1.0f;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	float diffuseStrength = 0.6f;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * diff * lightColor;

	// Specular
	float specularStrength = 0.2f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * Color.xyz;
	color = vec4(result, Color.w);
}
