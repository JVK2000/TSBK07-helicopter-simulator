/* 
Per-pixel calculations.
*/

#version 150

in vec2 textCoord;
in vec3 transformedNormal;
in vec4 surfacePositions;
in vec3 surfacePos;

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


void main(void)
{
	vec4 color = vec4(1, 1, 1, 1);
	mat3 camMat = mat3(cameraMatrix);
	vec3 normal_view = normalize(camMat * transformedNormal);

	if (shadingEnabled) {
		color = vec4(0, 0, 0, 0);

		//		for(int i = 3; i < 4; i++) {
		for(int i = 0; i < 4; i++) {
			vec3 lightDirection;
			if (isDirectional[i]) {
				// Directional light
				lightDirection = normalize(camMat * lightSourcesDirPosArr[i]);
			} else {
				// Positional light
				lightDirection = normalize(camMat *  (lightSourcesDirPosArr[i] - surfacePos));
			}
			// Diffuse light
			float diffuse = max(dot(normal_view, lightDirection), 0.0);
			vec3 diffuseLight = diffuse * lightSourcesColorArr[i] * 0.5;

			// Specular light
			vec3 viewDir = normalize(camMat * (cameraPos - surfacePos));
			vec3 reflectDir = reflect(-lightDirection, normalize(normal_view));
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
			vec3 specularLight = .8 * spec * lightSourcesColorArr[i];

			// color = color + vec4(diffuseLight, 1);
			color = color + vec4(diffuseLight + specularLight, 1);
		}
		// color = vec4(surfacePos, 1);
	}

	if (textureEnabled) {
        color = color * vec4(vec3(texture(texUnit, textCoord)), 1);
    }

	outColor = color;

	// outColor = vec4(camMat * surfacePos, 1);	// Scene visualizing surface positions
	// outColor = vec4(surfacePositions, 1);	// Scene visualizing surface positions
	// outColor = vec4(normal_view, 1);	// Scene visualizing normal

}


//	if (shadingEnabled) {
//		vec3 normal_view = normalize(mat3(cameraMatrix) * transformedNormal);
//		color = vec4(0, 0, 0, 0);
//
//		//		for(int i = 3; i < 4; i++) {
//		for(int i = 0; i < 4; i++) {
//			vec3 lightDirection;
//			if (isDirectional[i]) {
//				// Directional light
//				lightDirection = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[i]);
//			} else {
//				// Positional light
//				lightDirection = normalize(mat3(cameraMatrix) *  (lightSourcesDirPosArr[i] - surfacePos));
//			}
//			// Diffuse light
//			float diffuse = max(dot(normal_view, lightDirection), 0.0);
//			vec3 diffuseLight = diffuse * lightSourcesColorArr[i] * 0.5;
//
//			// Specular light
//			vec3 viewDir = normalize(mat3(cameraMatrix) * (cameraPos - surfacePos));
//			vec3 reflectDir = reflect(-lightDirection, normalize(normal_view));
//			float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
//			vec3 specularLight = .8 * spec * lightSourcesColorArr[i];
//
//			color = color + vec4(diffuseLight + specularLight, 1);
//		}
//	}
