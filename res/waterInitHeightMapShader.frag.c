#version 330 core

uniform sampler2D WaterHeightMap;

uniform float waterHeightMapDistance_W;
uniform float waterHeightMapDistance_H;
uniform float sin45;
uniform float attenuation;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vFragColor = vec4(0.0, 0.0, 0.0, 0.0);
}