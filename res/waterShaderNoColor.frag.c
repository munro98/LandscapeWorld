#version 330 core

uniform sampler2D WaterNormalMap;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vFragColor.rgb = texture(WaterNormalMap, TexCoord.st).rgb;
	vFragColor.r += 0.1;
	vFragColor.g -= 0.9;
	vFragColor.b += 0.1;
	vFragColor.a = 1;
}