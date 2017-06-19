#version 330 core

layout(location = 0) in vec3 position; 
layout(location = 1) in vec2 textureCoords;

uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection;
uniform sampler2D WaterHeightMap;

out vec2 TexCoord; 
out vec3 Position;

void main()
{
	TexCoord.st = vec2(position.x * 0.5 + 0.5, 0.5 - position.z * 0.5);
	Position = position.xyz;
	Position.y += texture(WaterHeightMap, TexCoord.st).r;
	gl_Position = projection * view * model * vec4(Position, 1.0);
}