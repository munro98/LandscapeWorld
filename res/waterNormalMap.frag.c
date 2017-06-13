// #version 120
#version 330

uniform sampler2D WaterHeightMap;

uniform float ODWNMR, WMSDWNMRM2;

in vec2 vVaryingTexCoord0;

out vec4 vFragColor;

void main()
{
	float y[4];

	y[0] = texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(ODWNMR, 0.0)).g;
	y[1] = texture(WaterHeightMap, vVaryingTexCoord0.st + vec2(0.0, ODWNMR)).g;
	y[2] = texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(ODWNMR, 0.0)).g;
	y[3] = texture(WaterHeightMap, vVaryingTexCoord0.st - vec2(0.0, ODWNMR)).g;

	vec3 Normal = normalize(vec3(y[2] - y[0], WMSDWNMRM2, y[1] - y[3]));

	vFragColor = vec4(Normal, 1.0);
}

// void main()
// {
	// float y[4];

	// y[0] = texture2D(WaterHeightMap, gl_TexCoord[0].st + vec2(ODWNMR, 0.0)).g;
	// y[1] = texture2D(WaterHeightMap, gl_TexCoord[0].st + vec2(0.0, ODWNMR)).g;
	// y[2] = texture2D(WaterHeightMap, gl_TexCoord[0].st - vec2(ODWNMR, 0.0)).g;
	// y[3] = texture2D(WaterHeightMap, gl_TexCoord[0].st - vec2(0.0, ODWNMR)).g;

	// vec3 Normal = normalize(vec3(y[2] - y[0], WMSDWNMRM2, y[1] - y[3]));

	// gl_FragColor = vec4(Normal, 1.0);
// }
