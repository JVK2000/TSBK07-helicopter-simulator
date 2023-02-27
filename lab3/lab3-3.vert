/* 	
Per-vertex calculations. 
*/
 
#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 normal;
out vec2 textCoord;
out vec3 transformedNormal;

uniform mat4 rotationMatrixY;
uniform mat4 rotationMatrixExtra;	// Used for the rotation of the windmill blades
uniform mat4 translationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
    transformedNormal = mat3(translationMatrix) * inNormal;
	
	
	gl_Position =  projectionMatrix * mdlMatrix * vec4(inPosition, 1.0);
	// gl_Position =  projectionMatrix  * cameraMatrix * translationMatrix * rotationMatrixExtra * vec4(inPosition, 1.0);
	
		
	textCoord = inTexCoord;
}
