/* 	
Per-vertex calculations. 
*/
 
#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 normal;
out vec4 textCoord;

uniform mat4 rotationMatrixZ;
uniform mat4 rotationMatrixX;
uniform mat4 translationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 camMatrix;


void main(void)
{
	gl_Position = projectionMatrix * translationMatrix * camMatrix * vec4(inPosition, 1.0);
	//gl_Position = projectionMatrix * translationMatrix * rotationMatrixZ * rotationMatrixX * vec4(inPosition, 1.0);
	normal = inNormal;
	
	textCoord = vec4(inTexCoord, 1.0, 1.0);
	//textCoord = rotationMatrixZ * rotationMatrixX * vec4(inTexCoord, 1.0, 1.0);
}