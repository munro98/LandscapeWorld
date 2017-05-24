#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;

in vec3 _toLightVector;

out vec4 color;


uniform sampler2D ourTexture1;
//uniform vec3 light_direction;
const vec3 light_direction = vec3(-0.5, 0.6, 0.7);
uniform vec3 lightPosition;
uniform vec3 lightColour;


uniform vec3 camPos;

void main()
{
	//vec2 tiledTexCoords = TexCoords * 40;
	vec2 tiledTexCoords = TexCoords;

	vec3 unitNormal = normalize(Normal);
	vec3 finalColour = vec3(0.0);

	float distance = length(_toLightVector);
	//vec3 unitLightVector = normalize(_toLightVector);
	vec3 unitLightVector = normalize(light_direction);
	float nDotl = dot(unitNormal, unitLightVector);
	nDotl = max(nDotl, 0.4);

	finalColour = finalColour + (nDotl * lightColour);

	vec3 texDiffuse = texture(ourTexture1, tiledTexCoords).xyz;

	color = vec4(finalColour * texDiffuse, 1.0);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}