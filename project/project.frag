#version 150

in vec2 texCoord;
in vec3 transformedNormal;
in vec3 surfacePos;
// in vec3 color;

out vec4 outColor;

uniform sampler2D tex;
uniform sampler2D tex2;

uniform bool textureEnabled;
uniform bool isHelicopter;
uniform bool isSky;
uniform bool specularLightEnabled;
uniform bool ambientEnabled;
uniform bool diffuseEnabled;

// 4 light sources
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

uniform mat4 mdlMatrix;
uniform vec3 cameraPos;
uniform vec4 helicopterColor;


void main(void)
{
	vec4 color = vec4(0, 0, 0, 1);
	mat3 camMat = mat3(mdlMatrix);
	vec3 normal_view = normalize(camMat * transformedNormal);
	vec4 baseColor = vec4(1, 1, 1, 1);

	if (isHelicopter) {
		baseColor = helicopterColor;
		// baseColor = vec4(0.325, 0.325, 0.325, 1);
	}

	if (diffuseEnabled || ambientEnabled || specularLightEnabled) {
		for(int i = 3; i < 4; i++) {
			vec3 lightDirection;
			if (isDirectional[i]) {
				// Directional light
				lightDirection = normalize(camMat * lightSourcesDirPosArr[i]);
			} else {
				// Positional light
				lightDirection = normalize(camMat *  (lightSourcesDirPosArr[i] - surfacePos));
			}

			// Diffuse light
			vec3 diffuseLight = vec3(0, 0, 0);
			if (diffuseEnabled) {
				float diffuse = max(dot(normal_view, lightDirection), 0.0);
				diffuseLight = diffuse * lightSourcesColorArr[i] * 0.6;
			}

			// Ambient light
			vec3 ambientLight = vec3(0, 0, 0);
			if (ambientEnabled) {
				float ambientStrength = 0.3;
				ambientLight = ambientStrength * lightSourcesColorArr[i];
			}

			// Specular light
			vec3 specularLight = vec3(0, 0, 0);
			if (specularLightEnabled) {
				vec3 viewDir = normalize(camMat * (cameraPos - surfacePos));
				vec3 reflectDir = reflect(-lightDirection, normalize(normal_view));
				float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
				specularLight = 0.8 * spec * lightSourcesColorArr[i];
			}

			vec4 temp_color = vec4(baseColor.rgb * (ambientLight + diffuseLight) + specularLight, 1);
			color = color + temp_color;
		}
	}

	if (isSky) {
		color = vec4(1, 1, 1, 1);
		color = color * texture(tex2, texCoord);
	}

	else if (textureEnabled) {
		// blendFactor - 0: 100% tex. 1: 100% tex2 
		float brake_point_1 = 0.4;
		float brake_point_2 = 0.7;
		float blendFactor;
		if (transformedNormal.y < brake_point_1) {
			blendFactor = 1;
		} else if (brake_point_2 < transformedNormal.y){
			blendFactor = 0;
		} else {
			blendFactor = (brake_point_2 - transformedNormal.y) * (1 / (brake_point_2 - brake_point_1));
		}		
		vec4 tot_tex = mix(texture(tex, texCoord), texture(tex2, texCoord), blendFactor);
        color = color * tot_tex;
    }

	outColor = color;

}

