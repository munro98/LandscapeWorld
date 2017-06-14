#version 330

uniform sampler2D WaterHeightMap;

uniform float waterHeightMapResolution_W;
uniform float waterHeightMapResolution_H;

in vec2 vVaryingTexCoord0;

out vec4 vFragColor;

void main()
{
	vec2 vh = texture(WaterHeightMap, vVaryingTexCoord0.st).rg;

	float force = 0.0;

	force += 0.707107 * (texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(waterHeightMapResolution_W, waterHeightMapResolution_H)).g - vh.g);
	force += texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(0.0, waterHeightMapResolution_H)).g - vh.g;
	force += 0.707107 * (texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(waterHeightMapResolution_W, -waterHeightMapResolution_H)).g - vh.g);

	force += texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(waterHeightMapResolution_W, 0.0)).g - vh.g;
	force += texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(waterHeightMapResolution_W, 0.0)).g - vh.g;

	force += 0.707107 * (texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(-waterHeightMapResolution_W, waterHeightMapResolution_H)).g - vh.g);
	force += texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(0.0, waterHeightMapResolution_H)).g - vh.g;
	force += 0.707107 * (texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(waterHeightMapResolution_W, waterHeightMapResolution_H)).g - vh.g);

	force *= 0.125;

	vh.r += force;
	vh.g += vh.r;
	vh.g *= 0.99;

	vFragColor = vec4(vh, 0.0, 0.0);
}