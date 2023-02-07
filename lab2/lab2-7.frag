/* 
Per-pixel calculations.
*/

#version 150

in vec3 normal;
in vec4 textCoord;
in vec3 transformedNormal; // Phong

out vec4 outColor;

uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(transformedNormal), light);

	// Linear mapping
	// float a = textCoord.s;
	// float b = textCoord.t;
	// outColor = texture(texUnit, vec2(a, b));
	outColor = vec4(shade, shade, shade, 1.0);
}
