#version 330 core

in vec3 vertex;
uniform mat4 pvmMatrix;

void main(void)
{
   gl_Position = pvmMatrix * vec4(vertex, 1.0);
}
