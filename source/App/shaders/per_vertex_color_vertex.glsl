#version 330 core

in vec3 vertex;
in vec4 color;

out vec4 Color;

uniform mat4 pvmMatrix;

void main(void)
{
	gl_Position = pvmMatrix * vec4(vertex, 1.0);

	Color = color;
}
