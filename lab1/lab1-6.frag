/* 
Fragment shaders perform per-pixel calculations. That's where texture 
and lighting colours are combined into one final pixel colour value.
 */

#version 150

in vec3 normal;

out vec4 out_Color;

void main(void)
{
	out_Color = vec4(normal, 1.0);
}
