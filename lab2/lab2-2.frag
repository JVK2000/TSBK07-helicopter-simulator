/* 
Per-pixel calculations.
*/

#version 150

in vec3 normal;
in vec4 textCoord;

out vec4 outColor;

uniform sampler2D texUnit;

void main(void)
{
	// Linear mapping
	float a = textCoord.s;
	float b = textCoord.t;
	outColor = texture(texUnit, vec2(a, b));
}
