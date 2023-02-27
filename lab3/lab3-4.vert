/* 	
Per-vertex calculations. 
*/
 
#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 textCoord;
out vec3 transformedNormal;
out vec3 surfacePos;

uniform mat4 translationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;
uniform mat4 mdlMatrix;


void main(void)
{
    transformedNormal = mat3(translationMatrix) * inNormal;
	
	gl_Position = projectionMatrix  * mdlMatrix * vec4(inPosition, 1.0);

	surfacePos = vec3(translationMatrix * vec4(inPosition, 1.0));

	textCoord = inTexCoord;
}
