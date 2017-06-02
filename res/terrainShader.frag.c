#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;

in vec3 _toLightVector;

in float visibility;

out vec4 color;


uniform sampler2D blend;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D stones;
uniform sampler2D snow;
//uniform vec3 light_direction;

uniform vec3 lightPosition;
uniform vec3 lightColour;

uniform float showBlendMap;
uniform float snowCoverage; //0.99

uniform vec3 camPos;

const vec3 light_direction = vec3(-0.5, 0.6, 0.7);
const vec3 skyColour = vec3(0.564, 0.682, 0.831);
const vec3 up = vec3(0.0, 1.0, 0);
//const float showBlendMap = 0.5f;


void main()
{
	vec2 tiledTexCoords = TexCoords * 20;

	vec4 blendColour = texture(blend, TexCoords);

	vec3 grassColour = texture(grass, tiledTexCoords).xyz * blendColour.g;
	vec3 rockColour = texture(rock, tiledTexCoords).xyz * blendColour.a;
	vec3 stonesColour = texture(stones, tiledTexCoords).xyz * blendColour.b;
	vec3 snowColour = texture(snow, tiledTexCoords).xyz * blendColour.r;

	vec3 texDiffuse = grassColour + snowColour + stonesColour + rockColour;

	vec3 unitNormal = normalize(Normal);

	if (dot(unitNormal, up) < 0.8) {
		texDiffuse = texture(rock, tiledTexCoords).rgb;
	}

	if (dot(unitNormal, up) > snowCoverage) {
		texDiffuse = texture(snow, tiledTexCoords).rgb;
	}

	texDiffuse = mix(texDiffuse, blendColour.rgb, showBlendMap);

	
	vec3 finalColour = vec3(0.0);

	float distance = length(_toLightVector);
	//vec3 unitLightVector = normalize(_toLightVector);
	vec3 unitLightVector = normalize(light_direction);
	float nDotl = dot(unitNormal, unitLightVector);
	nDotl = max(nDotl, 0.4);

	finalColour = finalColour + (nDotl * lightColour);

	//vec3 texDiffuse = texture(blend, TexCoords).xyz;
	//vec3 texDiffuse = texture(snow, tiledTexCoords).xyz;
	//mix(vec4(skyColour,1.0),out_Color, visibility);
	color = mix(vec4(skyColour,1.0), vec4(finalColour * texDiffuse, 1.0), visibility);

	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}