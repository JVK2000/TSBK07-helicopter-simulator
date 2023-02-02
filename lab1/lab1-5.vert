/* 	
Vertex shaders perform per-vertex calculations. That's where 
vertices are transformed, per-vertex lighting calculations are done, 
and where skeletal animation systems do most of their work.
 */
 
#version 150

in vec3 in_Position;
in  vec3 in_Color;

out vec3 color;

uniform mat4 rotationMatrixX;
uniform mat4 rotationMatrixY;
uniform mat4 rotationMatrixZ;


void main(void)
{
	gl_Position = rotationMatrixZ * rotationMatrixX * rotationMatrixY * vec4(in_Position, 1.0);
	color = in_Color;

}
