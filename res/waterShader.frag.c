#version 330 core
in vec2 TexCoord;
in vec3 surfaceNormal;

in vec3 toCameraVector;

in vec3 fromLightVector;

out vec4 color;

uniform sampler2D normalMap;


const vec3 lightDirection = vec3(-0.5, 0.6, 0.7);
const vec3 lightColour = vec3(1.4, 1.4, 1.4);


const vec4 skyColour = vec4(0.38, 0.504, 0.668, 1); //For reflections

const float waveStrength = 0.02;
const float shineDamper = 100.0;
const float reflectivity = 0.1;

void main()
{
	//color = texture(normalMap, TexCoord);
	vec4 normalMapColour = texture(normalMap, TexCoord);
	vec4 reflectColour = skyColour;

	vec3 normal = vec3(normalMapColour.r * 2.0 - 1.0, normalMapColour.b, normalMapColour.g * 2.0 - 1.0);

	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0,1.0,0.0));
	
	refractiveFactor = pow(refractiveFactor, 1);


	vec3 reflectedLight = reflect(normalize(-lightDirection), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = lightColour * specular * reflectivity;

	//color = vec4(specularHighlights, 1.0);
	color = vec4(reflectColour.x, reflectColour.y, reflectColour.z, clamp(refractiveFactor, 0.55, 1.0)) + vec4(specularHighlights, 0.0);
	//color = vec4(0.0f, 0.0f, 1.0f, 0.5f);
}