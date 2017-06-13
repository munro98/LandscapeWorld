#version 120

uniform sampler2D WaterNormalMap;

uniform vec3 LightPosition, CameraPosition;

varying vec3 Position;

void main()
{
	vec3 Normal = normalize(texture2D(WaterNormalMap, gl_TexCoord[0].st).rgb);
	//gl_FragColor.rgb = Normal;
	vec3 Direction = normalize(Position - CameraPosition);

	//vec3 ReflectedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, reflect(Direction, Normal))).rgb;
	//vec3 RefractedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, refract(Direction, Normal, 0.750395))).rgb;

	vec3 LightDirectionReflected = reflect(normalize(Position -  LightPosition), Normal);

	float Specular = pow(max(-dot(Direction, LightDirectionReflected), 0.0), 128);

	//gl_FragColor.rgb = mix(ReflectedColor, RefractedColor, -dot(Normal, Direction)) + Specular;
	gl_FragColor.rgb = normalize(LightDirectionReflected + Specular);
	//gl_FragColor = vec4(0,1,0, 0);

	// Test for simple Normal map
	// vec3 Normal = texture2D(WaterNormalMap, gl_TexCoord[0].st).rgb;
	// if(Normal.g == 1)
	// {
		// gl_FragColor.rgb = vec3(1,0,0);
	// }
	// else
	// {
		// gl_FragColor.rgb = vec3(0,1,0);
	// }
	// gl_FragColor.rgb = Normal;
	
	// Test for simple hight map
	// gl_FragColor.rgb = vec3(0,1,1);
	// gl_FragColor.r = abs(Position.y);
}
