#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 transformedNormal;
out vec3 surfacePos;

// NY
uniform mat4 translationMatrix;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 combinedMatrix;
 

void main(void)
{
	transformedNormal = mat3(translationMatrix) * inNormal;
	gl_Position = projMatrix  * mdlMatrix * vec4(inPosition, 1.0);
	surfacePos = vec3(translationMatrix * vec4(inPosition, 1.0));
	texCoord = inTexCoord;
}
