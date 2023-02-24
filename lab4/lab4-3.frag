#version 150

in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

uniform sampler2D tex;

void main(void)
{
	outColor = texture(tex, texCoord) * vec4(normal, 1);
}
