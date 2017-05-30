#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 surfaceNormal;

out vec3 toCameraVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;
uniform float deltaTime; // Use to animate textureCoords

void main()
{
	vec4 worldPosition = model * vec4(position, 1.0);
	gl_Position = projection * view * worldPosition;

	TexCoord = textureCoords;

	toCameraVector = cameraPosition - worldPosition.xyz;


}