/* 
Per-pixel calculations.
*/

#version 150

in vec3 normal;
in vec4 textCoord;
in float shade;

out vec4 outColor;

uniform sampler2D texUnit;

void main(void)
{
	outColor = vec4(shade, shade, shade, 1.0);
}
