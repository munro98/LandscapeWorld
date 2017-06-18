#version 330 core

in vec2 TexCoord;
in vec3 surfaceNormal;

in vec3 Position;

out vec4 color;

uniform sampler2D ourTexture1;

const float lowerLimit = 0.0;
const float upperLimit = 500.0;

void main()
{

	float factor = (Position.y- lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0, 1.0);
	
	//color = mix(vec4(0.72, 0.83, 0.925, 1), vec4(0.37, 0.494, 0.658, 1), factor);
	color = mix(vec4(0.75, 0.86, 1.00, 1), vec4(0.38, 0.504, 0.668, 1), factor);
}