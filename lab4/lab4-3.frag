#version 150

in vec2 texCoord;
in vec3 normal;
in vec3 surfacePos;
in vec2 textCoord;

out vec4 outColor;

uniform sampler2D tex;

uniform mat4 cameraMatrix;
uniform vec3 cameraPos;

uniform bool shadingEnabled;
uniform bool textureEnabled;

// 4 light sources
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

void main(void)
{
	vec4 color = vec4(1, 1, 1, 1);
	mat3 camMat = mat3(cameraMatrix);
	vec3 normal_view = normalize(camMat * normal);

	if (shadingEnabled) {
		color = vec4(0, 0, 0, 0);

		vec3 lightDirection = normalize(camMat * lightSourcesDirPosArr[3]);
		
		// Diffuse light
		float diffuse = max(dot(normal_view, lightDirection), 0.0);
		vec3 diffuseLight = diffuse * lightSourcesColorArr[3] * 0.5;

		color = color + vec4(diffuseLight, 1);

		// color = vec4(normal_view, 1);
	}

	if (textureEnabled) {
        color = color * vec4(vec3(texture(tex, texCoord)), 1);
    }

	outColor = color;


}
