/* 
Per-pixel calculations.
*/

#version 150

in vec3 normal;
in vec2 textCoord;
in vec3 transformedNormal; // Phong

out vec4 outColor;

uniform sampler2D texUnit;
uniform bool shadingEnabled;
uniform bool textureEnabled;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade = dot(normalize(transformedNormal), light);

	// Linear mapping
	// float a = textCoord.s;
	// float b = textCoord.t;
	vec4 color = vec4(1, 1, 1, 1);
	if (shadingEnabled) {
		color = shade * color; 
	} else {
	}

	if (textureEnabled) {
		color = color * texture(texUnit, textCoord);
	} else {
		
	}

	outColor = color;
	// 	if (shadingEnabled) {
	// 	outColor = shade * texture(texUnit, textCoord); 
	// } 
	// else {
	// 	outColor = texture(texUnit, textCoord); 
	// }

	// outColor = color
	// outColor = vec4(shade, shade, shade, 1.0);
}
