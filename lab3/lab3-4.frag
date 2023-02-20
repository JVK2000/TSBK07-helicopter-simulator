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
		
		// color = surfacePositions;	// Scene visualizing surface positions
		// color = vec4(normal, 1);	// Scene visualizing normal

		vec3 lightDir = normalize(lightSourcesDirPosArr[2]);

		float diff = max(dot(transformedNormal, lightDir), 0.0);			// how closely two vectors align, in terms of the directions they point.


		vec3 light = diff * lightSourcesColorArr[2] * 0.5;

		color = vec4(light, 1); 
		// color = vec4(lightSourcesColorArr[2] * lightSourcesDirPosArr[2], 1);
	}

	if (textureEnabled) {
        color = color * vec4(vec3(texture(texUnit, textCoord)), 1);
    } 

	outColor = color;
}
