/* 
Fragment shaders perform per-pixel calculations. That's where texture 
and lighting colours are combined into one final pixel colour value.
 */

#version 150

in vec3 normal;
in vec4 textCoord;

out vec4 out_Color;
out vec2 outTexCoord;


void main(void)
{
	out_Color = textCoord;
	// out_Color = vec4(vec3(textCoord, 1.0) * normal, 1.0);
}
