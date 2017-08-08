uniform mediump vec4 Color;
varying vec3 pos;
void main(void)
{
   gl_FragColor = Color;
   gl_FragColor.a = clamp(0.5 - Color.a * length(pos), 0, 1); // circle fading
   if(pos.x == 0) gl_FragColor.a *= 3.0; // z axis
   if(pos.z == 0) gl_FragColor.a *= 3.0; // x axis
   if (gl_FragColor.a < 0.01)discard;
}