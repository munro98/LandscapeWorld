#version 330

layout(location = 0) in vec3 vVertex; 

// attrib: standard texture coords
layout(location = 1) in vec2 vTexCoord0;

// output: tex coord
out vec2 vVaryingTexCoord0; 

uniform sampler2D WaterHeightMap;

out vec3 Position;
uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection;

void main()
{
	vVaryingTexCoord0.st = vec2(vVertex.x * 0.5 + 0.5, 0.5 - vVertex.z * 0.5);
	Position = vVertex.xyz;
	Position.y += texture(WaterHeightMap, vVaryingTexCoord0.st).g;
	gl_Position = projection * view * model * vec4(Position, 1.0);
}