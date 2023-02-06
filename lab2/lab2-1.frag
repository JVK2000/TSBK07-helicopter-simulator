/* 
Per-pixel calculations.
*/

#version 150

in vec3 normal;
in vec4 textCoord;

out vec4 out_Color;

void main(void)
{
	// Linear mapping
	float a = textCoord.s;
	float b = textCoord.t;
	out_Color = vec4(a, b, 1.0, 1.0);
}
