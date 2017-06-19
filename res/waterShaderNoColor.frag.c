#version 330 core

uniform sampler2D waterNormalMap;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vec3 normal = normalize(texture(waterNormalMap, TexCoord).rgb);
	vFragColor = vec4(normal.r + 0.1, normal.g - 0.9, normal.b + 0.1, 1);
}