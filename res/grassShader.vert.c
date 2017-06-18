#version 330 core 

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec3 normal;

out vec4 Position;
out vec3 Normal;
out float Time;
flat out int isVisible; //flag whether to render this instance or not

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

uniform float time;

void main() {
	Position = position+vec4(offset,0.0);
	Normal = normalize(normal);
	Time = time;
	isVisible = 1;

        if(offset.y < -60 || offset.y > 10){ //grass should only be rendered within 10 > y > -60
          isVisible = 0;
        }
}
