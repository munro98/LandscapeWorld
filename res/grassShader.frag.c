#version 330 core

in vec3 normal;
in vec3 viewDirection;

out vec4 color;

const float ambient = 0.2;
const vec3 light_direction = vec3(-0.5, 0.6, 0.7);
const vec3 grass_colour = vec3(0.388,0.560,0.223);

//TODO 

void main() {
	//vec3 ambientLight = ambient*light_colour

	float diffuse = dot(normal, normalize(light_direction));

        vec3 reflect = reflect(-normalize(light_direction), normal);

        float specular = pow(max(dot(viewDirection, reflect), 0.0), 32);

	color = vec4((ambient+diffuse+specular)*grass_colour, 0.0f);
}
