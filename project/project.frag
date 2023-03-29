#version 150

in vec2 texCoord;
in vec3 transformedNormal;
in vec3 surfacePos;

out vec4 outColor;

uniform sampler2D tex;
uniform sampler2D tex2;

uniform bool shadingEnabled;
uniform bool textureEnabled;
uniform bool isHelicopter;
uniform bool isSky;

// 4 light sources
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

uniform mat4 mdlMatrix;
uniform vec3 cameraPos;


void main(void)
{
	vec4 color = vec4(1, 1, 1, 1);
	mat3 camMat = mat3(mdlMatrix);
	vec3 normal_view = normalize(camMat * transformedNormal);

	if (shadingEnabled) {
		color = vec4(0, 0, 0, 0);

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
			vec3 diffuseLight = diffuse * lightSourcesColorArr[i] * 0.6;

			// Ambient light
			float ambientStrength = 0.2;
			vec3 ambientLight = ambientStrength * lightSourcesColorArr[i];
			ambientLight = vec3(0, 0, 0);

			// Specular light
			vec3 viewDir = normalize(camMat * (cameraPos - surfacePos));
			// vec3 viewDir = normalize(camMat * (- surfacePos));
			vec3 reflectDir = reflect(-lightDirection, normalize(normal_view));
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
			vec3 specularLight = 0.8 * spec * lightSourcesColorArr[i];

			color = color + vec4(diffuseLight + ambientLight + specularLight, 1);
			// color = color + vec4(diffuseLight + ambientLight, 1);
		}
	}

	if (isHelicopter) {
		color = color * texture(tex2, texCoord);
	}
	
	else if (isSky) {
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
		// vec4 tot_tex = vec4(vec3(texture(tex, texCoord)), 1);
        color = color * tot_tex;
    }

	// Fog parameters, could make them uniforms and pass them into the fragment shader
	// float fog_maxdist = 8.0;
	// float fog_mindist = 0.1;
	// vec4  fog_colour = vec4(0.4, 0.4, 0.4, 1.0);

	// Calculate fog
	// float dist = length(cameraPos.xyz);
	// float fog_factor = (fog_maxdist - dist) /
	// 				(fog_maxdist - fog_mindist);
	// fog_factor = clamp(fog_factor, 0.0, 1.0);
	// color = mix(fog_colour, color, fog_factor);


	outColor = color;


}
