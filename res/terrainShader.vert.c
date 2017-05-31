#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 WorldPos;

out vec3 _toLightVector;

out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 camPos;

uniform vec3 lightPosition;

const float density = 0.001;
const float gradient = 8.0;

void main()
{
	vec4 worldPosition = model * vec4(position,1.0);
	gl_Position = projection * view * worldPosition;
	Normal = mat3(model) * normal;
	WorldPos = worldPosition.xyz;

	float distance = length(camPos - worldPosition.xyz);
	
	visibility = exp(-pow((distance*density),gradient));
	visibility = clamp(visibility, 0.0, 1.0);
	//visibility = 1.0;

	TexCoords = textureCoords;

	_toLightVector = lightPosition - worldPosition.xyz;
}