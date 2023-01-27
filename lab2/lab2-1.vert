/* 	
Vertex shaders perform per-vertex calculations. That's where 
vertices are transformed, per-vertex lighting calculations are done, 
and where skeletal animation systems do most of their work.
 */
 
#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

out vec3 normal;
out vec4 textCoord;

uniform mat4 rotationMatrixZ;
uniform mat4 rotationMatrixX;

void main(void)
{
	gl_Position = rotationMatrixZ * rotationMatrixX * vec4(in_Position, 1.0);
	normal = in_Normal;

	// att rotating to get an animated texture (shifting colors)
	textCoord = rotationMatrixZ * rotationMatrixX * vec4(vec3(inTexCoord, 1.0), 1.0);
}
