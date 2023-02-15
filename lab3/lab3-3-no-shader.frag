/* 
Per-pixel calculations.
*/

#version 150

in vec3 normal;
in vec2 textCoord;
in vec3 transformedNormal; // Phong

out vec4 outColor;

uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade = dot(normalize(transformedNormal), light);

	outColor = texture(texUnit, textCoord);
	// outColor = shade * texture(texUnit, textCoord); 

}
