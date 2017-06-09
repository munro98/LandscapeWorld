#version 120

uniform sampler2D WaterNormalMap;

uniform vec3 LightPosition, CameraPosition;

varying vec3 Position;

void main()
{
	//if(texture2D(WaterNormalMap, vec2(1,1)).r < 0.2)
	//if(Position.y > 0.2)
	//{
	//	gl_FragColor.b = 1;
	//}
	//else
	//{
	//	gl_FragColor.g = 1;
	//}
	//vec3 Normal = texture2D(WaterNormalMap, gl_TexCoord[0].st).rgb;
	//gl_FragColor.rgb = Normal;
	gl_FragColor.rgb = vec3(0,1,0);
	//gl_FragColor.b = 1;

	//gl_FragColor.r = Position.y;

	//vec3 Normal = normalize(texture2D(WaterNormalMap, gl_TexCoord[0].st).rgb);
	//vec3 Direction = normalize(Position - CameraPosition);


}
