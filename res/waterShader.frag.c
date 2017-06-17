#version 330 core

uniform sampler2D WaterNormalMap;

uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform vec4 waterColour;

in vec3 Position;
in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	// lighting
	vec3 ambient  = vec3(0.1);
	vec3 diffuse  = vec3(0.0);
	vec3 specular = vec3(0.0);

	vec3 Normal = normalize(texture(WaterNormalMap, TexCoord.st).rgb);

	vec3 eye = normalize(CameraPosition - Position);
	vec3 light = normalize(LightPosition.xyz);
	vec3 halfVec = normalize(eye + light);
	// diffuse
	float diff = max(0.0, dot(Normal, light));
	diffuse += vec3(diff);
	// specular
	float spec = pow(max(0.0, dot(Normal, halfVec)), 64);
	specular += vec3(spec);

	vFragColor.rgb = waterColour.rgb * (ambient + diffuse) + specular;
	vFragColor.a = 1;
}