/* 	
Per-vertex calculations. 
*/
 
#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 normal;
out vec4 textCoord;
out vec3 transformedNormal;

uniform mat4 rotationMatrixY;
uniform mat4 rotationMatrixExtra;	// Used for the rotation of the windmill blades
uniform mat4 translationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;
uniform mat4 globalTransform;

void main(void)
{
	mat3 normalMatrix = mat3(translationMatrix);
    transformedNormal = normalMatrix * inNormal;
	
	gl_Position =  projectionMatrix  * cameraMatrix * globalTransform * translationMatrix * rotationMatrixExtra * vec4(inPosition, 1.0);
		
	textCoord = vec4(inTexCoord, 1.0, 1.0);
}
