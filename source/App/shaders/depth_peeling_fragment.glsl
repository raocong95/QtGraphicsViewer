#version 330 core

out vec4 color;

uniform vec2 windowSize;
uniform sampler2D depthTex;

void main(void)
{
	// Bit-exact comparison between FP32 z-buffer and fragment depth
	vec2 st = vec2(gl_FragCoord.x / windowSize.x, gl_FragCoord.y / windowSize.y);
	float frontDepth = texture2D(depthTex, st).r;
	if (gl_FragCoord.z <= frontDepth+0.005) {
		discard;
	}

	// Shade all the fragments behind the z-buffer
	float c = gl_FragCoord.z;
	color = vec4(c, c, c, 1);
}
