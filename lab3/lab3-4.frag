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

in vec3 surfPos;

void main(void)
{
	vec4 color = vec4(1, 1, 1, 1);

    if (shadingEnabled) {
//		 color = surfacePositions;	// Scene visualizing surface positions

		vec3 normal_view = mat3(cameraMatrix) * transformedNormal;


		// Positional diffuse light
		vec3 lightSourceDir0 = normalize(mat3(cameraMatrix) *  (lightSourcesDirPosArr[0] - surfPos));
		float diffuse0 = max(dot(normal_view, lightSourceDir0), 0.0);
		vec3 light0 = diffuse0 * lightSourcesColorArr[0] * 0.5;

		vec3 lightSourceDir1 = normalize(mat3(cameraMatrix) *  (lightSourcesDirPosArr[1] - surfPos));
		float diffuse1 = max(dot(normal_view, lightSourceDir1), 0.0);
		vec3 light1 = diffuse1 * lightSourcesColorArr[1] * 0.5;
		// ------	
		
		// Directional diffuse light
		vec3 lightSourceDir2 = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[2]);
		float diffuse2 = max(dot(normal_view, lightSourceDir2), 0.0);
		vec3 light2 = diffuse2 * lightSourcesColorArr[2] * 0.5;
		
		vec3 lightSourceDir3 = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[3]);
		float diffuse3 = max(dot(normal_view, lightSourceDir3), 0.0);
		vec3 light3 = diffuse3 * lightSourcesColorArr[3] * 0.5;
		// ------

		

//		color = vec4(light2, 1) + vec4(light3, 1);
		color = vec4(light0 + light1, 1);
	}

	if (textureEnabled) {
        color = color * vec4(vec3(texture(texUnit, textCoord)), 1);
    }

	outColor = color;
}
