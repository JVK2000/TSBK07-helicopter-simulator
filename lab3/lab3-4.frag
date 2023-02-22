/* 
Per-pixel calculations.
*/

#version 150

in vec2 textCoord;
in vec3 transformedNormal; // Phong
in vec4 surfacePositions;

out vec4 outColor;

uniform sampler2D texUnit;
uniform bool shadingEnabled;
uniform bool textureEnabled;

// 4 light sources
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

uniform mat4 cameraMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPos;

in vec3 fragPos;

void main(void)
{
	vec4 color = vec4(1, 1, 1, 1);

    if (shadingEnabled) {
//		 color = surfacePositions;	// Scene visualizing surface positions
		vec3 normal_view = mat3(cameraMatrix) * transformedNormal;
		color = vec4(0, 0, 0, 0);

//		for(int i = 3; i < 4; i++) {
		for(int i = 0; i < 4; i++) {
			vec3 lightDirection;
			if (isDirectional[i]) {
				// Directional light
				lightDirection = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[i]);
			} else {
				// Positional light
				lightDirection = normalize(mat3(cameraMatrix) *  (lightSourcesDirPosArr[i] - fragPos));
			}
			// Diffuse light
			float diffuse = max(dot(normal_view, lightDirection), 0.0);
			vec3 diffuseLight = diffuse * lightSourcesColorArr[i] * 0.5;

			// Specular light
			vec3 viewDir = normalize(mat3(cameraMatrix) * (cameraPos - fragPos));
			vec3 reflectDir = reflect(-lightDirection, normalize(normal_view));
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
			vec3 specularLight = .8 * spec * lightSourcesColorArr[i];

			color = color + vec4(diffuseLight + specularLight, 1);

		}
	}

	if (textureEnabled) {
        color = color * vec4(vec3(texture(texUnit, textCoord)), 1);
    }

	outColor = color;
}
