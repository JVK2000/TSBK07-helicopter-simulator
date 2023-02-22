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
		vec3 light, lightDirection;
		vec4 totLight = vec4(0, 0, 0, 0);
		vec3 lightDirSpecular;

//		for(int i = 3; i < 4; i++) {
		for(int i = 0; i < 4; i++) {

			if (isDirectional[i]) {
				// Directional diffuse light
				lightDirection = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[i]);
				lightDirSpecular = normalize(lightSourcesDirPosArr[i]);

			} else {
				// Positional diffuse light
				lightDirection = normalize(mat3(cameraMatrix) *  (lightSourcesDirPosArr[i] - fragPos));
				lightDirSpecular = normalize(lightSourcesDirPosArr[i] - fragPos);
			}
			float diffuse = max(dot(normal_view, lightDirection), 0.0);
			light = diffuse * lightSourcesColorArr[i] * 0.5;

			// Specular light
			vec3 viewDir = normalize(cameraPos - fragPos);
			vec3 reflectDir = reflect(-lightDirSpecular, normalize(transformedNormal));
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
			vec3 specular = .8 * spec * lightSourcesColorArr[i];
			light = light + specular;
			// ------
			totLight = totLight + vec4(light, 1);

		}

		color = totLight;
//		color = vec4(light0 + light1 + light2 + light3, 1);
	}

	if (textureEnabled) {
        color = color * vec4(vec3(texture(texUnit, textCoord)), 1);
    }

	outColor = color;
}
