#version 330 core

uniform sampler2D waterHeightMap;

uniform float dropRadius;
uniform vec2 dropPosition;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vec2 currentHeight  = texture(waterHeightMap, TexCoord).xy;
	vFragColor = vec4(currentHeight.r - 4.0f * max(dropRadius - distance(TexCoord.xy, dropPosition), 0.0), currentHeight.g, 0.0, 0.0);
}