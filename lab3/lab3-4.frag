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


void main(void)
{
	vec4 color = vec4(1, 1, 1, 1);

    if (shadingEnabled) {
		
		// local vector  = fsurfacePositions -  light source pos.

		// color = surfacePositions;	// Scene visualizing surface positions
		// color = vec4(mat3(cameraMatrix) * transformedNormal, 1);	// Scene visualizing normal

		// Directional diffuse light
		vec3 lightSource2Dir = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[2]);
		vec3 lightSource3Dir = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[3]);

		// från https://learnopengl.com/Lighting/Basic-Lighting
		vec3 normal_view_coord = mat3(cameraMatrix) * transformedNormal;
		float diffuse2 = max(dot(normal_view_coord, lightSource2Dir), 0.0);	// dot product is how closely two vectors align, in terms of the directions they point.
		float diffuse3 = max(dot(normal_view_coord, lightSource3Dir), 0.0);	

		vec3 light2 = diffuse2 * lightSourcesColorArr[2] * 0.5;
		vec3 light3 = diffuse3 * lightSourcesColorArr[3] * 0.5;
		// ------

		// Positional diffuse light
		// vec3 locVec0 = vec3(surfacePositions) - lightSourcesDirPosArr[0];
		// vec3 light0 = locVec0 * lightSourcesColorArr[0] * 0.5;
		vec3 locVec0 = vec3(surfacePositions) - lightSourcesDirPosArr[0];
		vec3 light0dd = locVec0 * lightSourcesColorArr[0] * 0.5;

		float diffuse0 = max(dot(normal_view_coord, light0dd), 0.0);	

		vec3 light0 = diffuse0 * lightSourcesColorArr[0] * 0.5;

		// ------





		color = vec4(light2, 1) + vec4(light3, 1) + vec4(light0, 1); 
	}

	if (textureEnabled) {
        color = color * vec4(vec3(texture(texUnit, textCoord)), 1);
    } 

	outColor = color;
}
