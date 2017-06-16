#version 330 core 

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 offset;

out vec4 Position;
flat out int isVisible;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

void main() {
	Position = position+vec4(offset,0.0);
	isVisible = 1;

        if(offset.y < -59 || offset.y > 10){
          isVisible = 0;
        }
}
