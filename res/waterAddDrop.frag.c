// #version 120

// uniform sampler2D WaterHeightMap;

// uniform float DropRadius;
// uniform vec2 Position;

// void main()
// {
	// vec2 vh = texture2D(WaterHeightMap, gl_TexCoord[0].st).rg;

	// float d = distance(gl_TexCoord[0].st, Position);
	// //float d = distance(gl_TexCoord[0].st, vec2(0,0));

	// gl_FragColor = vec4(vh.r, vh.g - 4.0f * max(DropRadius - d, 0.0), 0.0, 0.0);
	// //gl_FragColor = vec4(0, 1, 0.0, 0.0);
// }

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
	// //
	// // get the change of height from four neightbours
	// //
	// float y  = data.r;
	// float yn = texture(texture0, vVaryingTexCoord0.xy + vec2(0.0, texelSize.y)).r  - y;
	// float yw = texture(texture0, vVaryingTexCoord0.xy + vec2(texelSize.x, 0.0)).r  - y;
	// float ys = texture(texture0, vVaryingTexCoord0.xy + vec2(0.0, -texelSize.y)).r - y;
	// float ye = texture(texture0, vVaryingTexCoord0.xy + vec2(-texelSize.x, 0.0)).r - y;

	// // add to the current 'velocity'
	// data.g  += (yn + yw + ys + ye) * density.y;
   
	// // reduce the speed a bit
	// data.g *=  density.x;
	
	// // move the 'height', but not with full speed
	// data.r = (data.r + data.g) * density.z;
	
	// vFragColor = vec4(data.r, data.g, 0.0, 0.0);
}