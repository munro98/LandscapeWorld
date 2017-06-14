#version 330 core

//in vec3 normal;

out vec4 color;

const float ambient = 0.1;
const vec3 light_direction = vec3(-0.5, 0.6, 0.7);
const vec3 light_colour = vec3(1.4, 1.4, 1.4);
const vec3 grass_colour = vec3(0.0,0.5,0.0);

//TODO 

void main() {
	vec3 ambientLight = ambient*light_colour;
	color = vec4(ambientLight*grass_colour, 1.0f);
}
