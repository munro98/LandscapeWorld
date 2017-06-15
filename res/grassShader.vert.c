#version 330 core 

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

void main() {
	gl_Position = vec4(position+offset, 1.0);
}
