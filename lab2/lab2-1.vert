/* 	
Per-vertex calculations. 
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
	
	textCoord = vec4(inTexCoord, 1.0, 1.0);
	//textCoord = rotationMatrixZ * rotationMatrixX * vec4(inTexCoord, 1.0, 1.0);
}
