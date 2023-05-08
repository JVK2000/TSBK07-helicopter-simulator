#version 150

in vec2 texCoord;
in vec3 transformedNormal;
in vec3 surfacePos;

out vec4 outColor;

uniform sampler2D tex;
uniform sampler2D tex2;

uniform bool textureEnabled;
uniform bool isHelicopter;
uniform bool isSky;
uniform bool specularLightEnabled;
uniform bool ambientLightEnabled;
uniform bool diffuseLightEnabled;

// 1 light sources
uniform vec3 lightSourcesDirPosArr[1];
uniform vec3 lightSourcesColorArr[1];

uniform mat4 mdlMatrix;
uniform vec4 helicopterColor;

uniform float dayTimeBlender;
uniform float diffuseStrength;
uniform float ambientStrength;


void main(void)
{
	vec4 color = vec4(0, 0, 0, 1);
	mat3 camMat = mat3(mdlMatrix);
	vec3 normalView = normalize(camMat * transformedNormal);
	vec4 baseColor = vec4(1, 1, 1, 1);

	if (isHelicopter) {
		baseColor = helicopterColor;
	}

	if (diffuseLightEnabled || ambientLightEnabled || specularLightEnabled) {
		for(int i = 0; i < 1; i++) {

			// Directional light
			vec3 lightDirection = normalize(camMat * lightSourcesDirPosArr[i]);

			// Diffuse light
			vec3 diffuseLight = vec3(0, 0, 0);
			if (diffuseLightEnabled) {
				float diffuse = max(dot(normalView, lightDirection), 0.0);
				diffuseLight = diffuse * lightSourcesColorArr[i] * diffuseStrength;
			}

			// Ambient light
			vec3 ambientLight = vec3(0, 0, 0);
			if (ambientLightEnabled) {
				ambientLight = ambientStrength * lightSourcesColorArr[i];
			}

			vec4 temp_color = vec4(baseColor.rgb * (ambientLight + diffuseLight), 1);
			color = color + temp_color;
		}
	}

	if (isSky) {
		color = vec4(1, 1, 1, 1);
		color = mix(texture(tex, texCoord), texture(tex2, texCoord), dayTimeBlender);
	}

	else if (textureEnabled) {
		// blendFactor - 0: 100% tex. 1: 100% tex2 
		float brakePoint1 = 0.4;
		float brakePoint2 = 0.7;
		float blendFactor;
		if (transformedNormal.y < brakePoint1) {
			blendFactor = 1;
		} else if (brakePoint2 < transformedNormal.y){
			blendFactor = 0;
		} else {
			blendFactor = (brakePoint2 - transformedNormal.y) * (1 / (brakePoint2 - brakePoint1));
		}		
		vec4 tot_tex = mix(texture(tex, texCoord), texture(tex2, texCoord), blendFactor);
        color = color * tot_tex;
    }

	outColor = color;
}

