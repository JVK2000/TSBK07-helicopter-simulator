/* 
Per-pixel calculations.
*/

#version 150

in vec2 textCoord;
in vec3 transformedNormal; // Phong

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


void main(void)
{
	vec3 color = vec3(1, 1, 1);

    if (shadingEnabled) {
		color = vec3(0, 0, 0);

		for(int i = 0; i < 4; i++){

			vec3 light = (mat3(cameraMatrix) * normalize(lightSourcesDirPosArr[i]));
			float shade = dot(normalize(transformedNormal),light);

			color += vec3(shade) * lightSourcesColorArr[i];
		}
	}

	if (textureEnabled) {
        color = color * vec3(texture(texUnit, textCoord));
    } 

	outColor = vec4(color, 1);
}
