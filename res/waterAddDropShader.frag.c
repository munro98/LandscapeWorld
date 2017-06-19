#version 330 core

uniform sampler2D WaterHeightMap;

uniform float DropRadius;
uniform vec2 DropPosition;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vec2 currentHeight  = texture(WaterHeightMap, TexCoord).xy; 
	vFragColor = vec4(currentHeight.r - 4.0f * max(DropRadius - distance(TexCoord.xy, DropPosition), 0.0), currentHeight.g, 0.0, 0.0);
}