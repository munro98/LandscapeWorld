#version 330 core

uniform sampler2D WaterNormalMap;

uniform float waterNormalMapDistance_W;
uniform float waterNormalMapDistance_H;
uniform float defaultHeight;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	// get the neighbouring y coordinates (height)
	float bottomHeight = texture(WaterNormalMap, TexCoord.st - vec2(waterNormalMapDistance_W, 0.0)).r;
	float topHeight = texture(WaterNormalMap, TexCoord.st + vec2(waterNormalMapDistance_W, 0.0)).r;
	float rightHeight = texture(WaterNormalMap, TexCoord.st + vec2(0.0, waterNormalMapDistance_H)).r;
	float leftHeight = texture(WaterNormalMap, TexCoord.st - vec2(0.0, waterNormalMapDistance_H)).r;

	// Create new normal vector, averaging the top/bottom height (x), the defaultHeight (y) and the average of left/right (z)
	vec3 Normal = normalize(vec3(bottomHeight - topHeight, defaultHeight, rightHeight - leftHeight));

	vFragColor = vec4(Normal, 1.0);
}