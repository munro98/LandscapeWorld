#version 330

uniform sampler2D WaterNormalMap;

uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform vec4 waterColor;

in vec3 Position;
in vec2 vVaryingTexCoord0;

out vec4 vFragColor;

void main()
{
	// // read normal
	// vec3 texNorm = texture(normalMap, vertexIn.vTexCoord0).rgb;
	// vec3 norm = normalize(texNorm*2.0-1.0);

	// // lighting
	// vec3 ambient  = vec3(0.1);
	// vec3 diffuse  = vec3(0.0);
	// vec3 specular = vec3(0.0);
	
	// vec3 eye = normalize(vertexIn.vEyePosTS - vertexIn.vPosTS);
	// vec3 light = normalize(vertexIn.vLightPosTS - vertexIn.vPosTS);
	
	// vec3 halfv = normalize(eye + light);

	// // diffuse
	// float diffElem = max(0.0, dot(norm, light));
	// diffuse += vec3(diffElem);

	// // specular
	// float specularElem = pow(max(0.0, dot(norm, halfv)), 64.0);
	// specular = vec3(specularElem);

	// // final
	// vec3 tex = texture(texture0, vertexIn.vTexCoord0+norm.xy*refractionFactor).rgb;

    // vFragColor.rgb = waterColor.rgb * tex * (ambient + diffuse) + specular;
	// vFragColor.a = waterColor.a;
//-----------------------------------------
	// lighting
	vec3 ambient  = vec3(0.1);
	vec3 diffuse  = vec3(0.0);
	vec3 specular = vec3(0.0);

	vec3 Normal = normalize(texture(WaterNormalMap, vVaryingTexCoord0.st).rgb);
	//gl_FragColor.rgb = Normal;
	vec3 eye = normalize(CameraPosition - Position);
	vec3 light = normalize(LightPosition - Position);
	vec3 halfVec = normalize(eye + light);
	// diffuse
	float diff = max(0.0, dot(Normal, light));
	diffuse += vec3(diff);
	// specular
	float spec = pow(max(0.0, dot(Normal, halfVec)), 64);
	specular += vec3(spec);

	vFragColor.rgb = waterColor.rgb * (ambient + diffuse) + specular;
	vFragColor.a = waterColor.a;
	
	// //vec3 Direction = normalize(Position - CameraPosition);

	// //vec3 ReflectedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, reflect(Direction, Normal))).rgb;
	// //vec3 RefractedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, refract(Direction, Normal, 0.750395))).rgb;

	// // vec3 ReflectedColor = vec3(255.0 / 112.0, 255.0 / 143.0, 255.0 / 184.0);
	// vec3 ReflectedColor = vec3(112.0 / 255.0, 143.0 / 255.0, 184.0 / 255.0);
	// vec3 LightDirectionReflected = reflect(normalize(Position -  LightPosition), Normal);

	// float Specular = pow(max(-dot(Direction, LightDirectionReflected), 0.0), 128);

	// //gl_FragColor.rgb = mix(ReflectedColor, RefractedColor, -dot(Normal, Direction)) + Specular;
	// //gl_FragColor.rgb = mix(LightDirectionReflected, ReflectedColor, -dot(Normal, Direction)) + Specular;
	// // vFragColor.rgb = normalize(ReflectedColor + Specular);
	// vFragColor.rgb = normalize(LightDirectionReflected + Specular);
	// //gl_FragColor.rgb = ReflectedColor + Specular;
	// //gl_FragColor = vec4(0,1,0, 0);
}

// #version 120

// uniform sampler2D WaterNormalMap;

// uniform vec3 LightPosition, CameraPosition;

// varying vec3 Position;

// void main()
// {
	// vec3 Normal = normalize(texture2D(WaterNormalMap, gl_TexCoord[0].st).rgb);
	// //gl_FragColor.rgb = Normal;
	// vec3 Direction = normalize(Position - CameraPosition);

	// //vec3 ReflectedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, reflect(Direction, Normal))).rgb;
	// //vec3 RefractedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, refract(Direction, Normal, 0.750395))).rgb;

	// // vec3 ReflectedColor = vec3(255.0 / 112.0, 255.0 / 143.0, 255.0 / 184.0);
	// vec3 ReflectedColor = vec3(112.0 / 255.0, 143.0 / 255.0, 184.0 / 255.0);
	// vec3 LightDirectionReflected = reflect(normalize(Position -  LightPosition), Normal);

	// float Specular = pow(max(-dot(Direction, LightDirectionReflected), 0.0), 128);

	// //gl_FragColor.rgb = mix(ReflectedColor, RefractedColor, -dot(Normal, Direction)) + Specular;
	// //gl_FragColor.rgb = mix(LightDirectionReflected, ReflectedColor, -dot(Normal, Direction)) + Specular;
	// gl_FragColor.rgb = normalize(ReflectedColor + Specular);
	// //gl_FragColor.rgb = ReflectedColor + Specular;
	// //gl_FragColor = vec4(0,1,0, 0);

	// // Test for simple Normal map
	// // vec3 Normal = texture2D(WaterNormalMap, gl_TexCoord[0].st).rgb;
	// // if(Normal.g == 1)
	// // {
		// // gl_FragColor.rgb = vec3(1,0,0);
	// // }
	// // else
	// // {
		// // gl_FragColor.rgb = vec3(0,1,0);
	// // }
	// // gl_FragColor.rgb = Normal;
	
	// // Test for simple hight map
	// // gl_FragColor.rgb = vec3(0,1,1);
	// // gl_FragColor.r = abs(Position.y);
// }
