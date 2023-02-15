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

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade = dot(normalize(transformedNormal), light);

	// Linear mapping
	// float a = textCoord.s;
	// float b = textCoord.t;
	float color;
	if (shadingEnabled) {
		outColor = shade * texture(texUnit, textCoord); 
	} 
	else {
		outColor = texture(texUnit, textCoord); 
	}

	// outColor = color
	// outColor = vec4(shade, shade, shade, 1.0);
}
