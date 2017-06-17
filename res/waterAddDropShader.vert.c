#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec2 TexCoord;    

void main() 
{
	TexCoord = textureCoords;
	gl_Position = vec4(position.xy, 0.0, 1.0);         
}