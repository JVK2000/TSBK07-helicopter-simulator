#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 normal;
out vec3 surfacePos;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 modelView;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;
	normal = inNormal;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
	surfacePos = vec3(modelView * vec4(inPosition, 1.0));
}
