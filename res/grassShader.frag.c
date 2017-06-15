#version 330 core

in vec3 normal_out;

out vec4 color;

const float ambient = 0.1;
const vec3 light_direction = vec3(-0.5, 0.6, 0.7);
//const vec3 light_colour = vec3(0.7, 0.7, 0.7);
const vec3 grass_colour = vec3(0.0,1.0,0.0);

//TODO 

void main() {
	//vec3 ambientLight = ambient*light_colour;

	float diffuse = dot(normalize(normal_out), normalize(light_direction));

	color = vec4((ambient+diffuse)*grass_colour, 0.0f);
}
