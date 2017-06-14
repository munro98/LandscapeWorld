#version 330

uniform sampler2D WaterHeightMap;

uniform float DropRadius;
uniform vec2 Position;

// input: standard texture coord
in vec2 vVaryingTexCoord0;

out vec4 vFragColor;

void main()
{
	vec2 data  = texture(WaterHeightMap, vVaryingTexCoord0.xy).rg; 
	float d = distance(vVaryingTexCoord0.xy, Position);
	vFragColor = vec4(data.r, data.g - 4.0f * max(DropRadius - d, 0.0), 0.0, 0.0);
}