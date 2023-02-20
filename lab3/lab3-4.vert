/* 	
Per-vertex calculations. 
*/
 
#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 textCoord;
out vec3 transformedNormal;
out vec4 surfacePositions;
out vec3 normal;

uniform mat4 rotationMatrixExtra;	// Used for the rotation of the windmill blades
uniform mat4 translationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;


void main(void)
{
	mat3 normalMatrix = mat3(translationMatrix);
    transformedNormal = normalMatrix * inNormal;
	normal = mat3(cameraMatrix) *  inNormal;
	
	gl_Position =  projectionMatrix  * cameraMatrix * translationMatrix * rotationMatrixExtra * vec4(inPosition, 1.0);
	surfacePositions = 	 cameraMatrix * translationMatrix * rotationMatrixExtra * vec4(inPosition, 1.0);

	textCoord = inTexCoord;
}