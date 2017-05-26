#version 330 core
in vec2 TexCoord;
in vec3 surfaceNormal;

out vec4 color;

uniform sampler2D ourTexture1;

const vec3 light_direction = vec3(-0.5, 0.6, 0.7);
const vec3 light_colour = vec3(1.4, 1.4, 1.4);

void main()
{
	color = texture(ourTexture1, TexCoord);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}