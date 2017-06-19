#version 330 core

uniform sampler2D waterNormalMap;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec4 waterColour;

in vec3 Position;
in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vec3 Normal = normalize(texture(waterNormalMap, TexCoord.st).rgb);

	// diffuse
	vec3 lightDirection = normalize(lightPosition);
	vec3 diffuse = vec3(max(dot(Normal, lightDirection), 0.0));

	// specular
	vec3 viewDirection = normalize(cameraPosition - Position);
	vec3 halfVec = normalize(viewDirection + lightDirection);
	vec3 specular = vec3(pow(max(0.0, dot(Normal, halfVec)), 64));

	vFragColor = vec4(waterColour.rgb * (vec3(0.1) + diffuse) + specular, 1);
}