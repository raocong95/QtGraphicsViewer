attribute highp vec4 vertex;
uniform highp mat4 pvmMatrix;
varying vec3 pos;
void main(void)
{
   gl_Position = pvmMatrix * vertex;
   pos = vertex.xyz;
}