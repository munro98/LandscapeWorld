#version 330

uniform sampler2D WaterHeightMap;

uniform float ODWHMR;

in vec2 vVaryingTexCoord0;

out vec4 vFragColor;

void main()
{
	vec2 vh = texture(WaterHeightMap, vVaryingTexCoord0.st).rg;

	float force = 0.0;

	force += 0.707107 * (texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(ODWHMR, ODWHMR)).g - vh.g);
	force += texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(0.0, ODWHMR)).g - vh.g;
	force += 0.707107 * (texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(ODWHMR, -ODWHMR)).g - vh.g);

	force += texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(ODWHMR, 0.0)).g - vh.g;
	force += texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(ODWHMR, 0.0)).g - vh.g;

	force += 0.707107 * (texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(-ODWHMR, ODWHMR)).g - vh.g);
	force += texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(0.0, ODWHMR)).g - vh.g;
	force += 0.707107 * (texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(ODWHMR, ODWHMR)).g - vh.g);

	force *= 0.125;

	vh.r += force;
	vh.g += vh.r;
	vh.g *= 0.99;

	vFragColor = vec4(vh, 0.0, 0.0);
}