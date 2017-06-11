#version 120

uniform sampler2D WaterHeightMap;

varying vec3 Position;
uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection;

void main()
{
	gl_TexCoord[0].st = vec2(gl_Vertex.x * 0.5 + 0.5, 0.5 - gl_Vertex.z * 0.5);
	Position = gl_Vertex.xyz;
	Position.y += texture2D(WaterHeightMap, gl_TexCoord[0].st).g;
	gl_Position = projection * view * model * vec4(Position, 1.0);
	//gl_Position = gl_ModelViewProjectionMatrix * vec4(Position, 1.0);
}