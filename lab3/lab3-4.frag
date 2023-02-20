/* 
Per-pixel calculations.
*/

#version 150

in vec2 textCoord;
in vec3 transformedNormal; // Phong
in vec4 surfacePositions;
in vec3 normal;

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
		// color = vec4(normal, 1);	// Scene visualizing normal

		vec3 lightSource2Dir = normalize(lightSourcesDirPosArr[2]);			// ska ändra till view kordinatsystem
		vec3 lightSource3Dir = normalize(lightSourcesDirPosArr[3]);

		// från https://learnopengl.com/Lighting/Basic-Lighting
		float diffuse2 = max(dot(transformedNormal, lightSource2Dir), 0.0);	// dot product is how closely two vectors align, in terms of the directions they point.
		float diffuse3 = max(dot(transformedNormal, lightSource3Dir), 0.0);	

		// ska använda normal istället för transformedNormal

		vec3 light2 = diffuse2 * lightSourcesColorArr[2] * 0.5;
		vec3 light3 = diffuse3 * lightSourcesColorArr[3] * 0.5;

		color = vec4(light2, 1) + vec4(light3, 1); 
	}

	if (textureEnabled) {
        color = color * vec4(vec3(texture(texUnit, textCoord)), 1);
    } 

	outColor = color;
}
