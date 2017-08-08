#version 330 core

in vec3 vertex;
in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

void main(void)
{
   gl_Position = viewMatrix * modelMatrix * vec4(vertex, 1.0);

   vec4 N = normalize(normalMatrix * vec4(normal, 0.0));
   Normal = N.xyz;

   FragPos = vec3(modelMatrix * vec4(vertex, 1.0f));
}
