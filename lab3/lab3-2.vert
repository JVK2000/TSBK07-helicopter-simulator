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
uniform mat4 rotationMatrixExtra;
uniform mat4 translationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;
uniform mat4 globalTransform;
uniform mat4 cameraMatrixRotation;
uniform mat4 cameraMatrixPos;

void main(void)
{
	mat3 normalMatrix = mat3(projectionMatrix * translationMatrix * cameraMatrix);
    transformedNormal = normalMatrix * inNormal;
	
	// An matrix is affected by it right neigbour. 
	// gl_Position = projectionMatrix * cameraMatrix * globalTransform * rotationMatrixY * translationMatrix * rotationMatrixExtra * vec4(inPosition, 1.0);
	gl_Position =  projectionMatrix  * cameraMatrixRotation * cameraMatrixPos * globalTransform * cameraMatrix * /*rotationMatrixY * */translationMatrix * rotationMatrixExtra * vec4(inPosition, 1.0);
	
	// If the models should rotates aswell.
	//gl_Position = projectionMatrix * cameraMatrix * rotationMatrixY * translationMatrix * rotationMatrixY * vec4(inPosition, 1.0);
	
	textCoord = vec4(inTexCoord, 1.0, 1.0);
}
