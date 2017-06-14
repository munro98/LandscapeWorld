#version 330

uniform sampler2D WaterHeightMap;

uniform float waterNormalMapResolution_W;
uniform float waterNormalMapResolution_H;
uniform float WMSDWNMRM2;

in vec2 vVaryingTexCoord0;

out vec4 vFragColor;

void main()
{
	float y[4];

	y[0] = texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(waterNormalMapResolution_W, 0.0)).g;
	y[1] = texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(0.0, waterNormalMapResolution_H)).g;
	y[2] = texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(waterNormalMapResolution_W, 0.0)).g;
	y[3] = texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(0.0, waterNormalMapResolution_H)).g;

	vec3 Normal = normalize(vec3(y[2] - y[0], WMSDWNMRM2, y[1] - y[3]));

	vFragColor = vec4(Normal, 1.0);
}