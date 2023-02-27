/* 	
Per-vertex calculations. 
*/
 
#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 textCoord;
out vec3 transformedNormal;
out vec3 surfacePositions;

uniform mat4 rotationMatrixExtra;	// Used for the rotation of the windmill blades
uniform mat4 translationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;
uniform mat4 mdlMatrix;

out vec3 surfacePos;
out vec3 pos_to_fragment;

void main(void)
{
	mat3 normalMatrix = mat3(translationMatrix);
    transformedNormal = normalMatrix * inNormal;
	
	gl_Position = projectionMatrix  * mdlMatrix * vec4(inPosition, 1.0);

	surfacePositions = vec3(mdlMatrix * vec4(inPosition, 1.0));
	surfacePos = vec3(translationMatrix * vec4(inPosition, 1.0));


	textCoord = inTexCoord;
}
