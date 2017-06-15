#version 330 core

in vec3 normal_out;

out vec4 color;

const float ambient = 0.1;
const vec3 light_direction = vec3(-0.5, 0.6, 0.7);
const vec3 light_colour = vec3(1.0, 1.0, 1.0);
const vec3 grass_colour = vec3(0.0,0.5,0.0);

//TODO 

void main() {
	vec3 ambientLight = ambient*light_colour;
	vec3 diffuseLight = dot(normal_out, light_direction)*light_colour;
	color = vec4((ambientLight+diffuseLight)*grass_colour, 1.0f);
}
