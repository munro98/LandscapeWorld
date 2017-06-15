#version 330 core 

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 offset;

//flat out int isVisible;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

void main() {
	//isVisible = 1;
	gl_Position = position+vec4(offset,0.0);
	//culling checks
	//if(offset.y > 10 || offset.y < -59){ isVisible = 0;}
}
