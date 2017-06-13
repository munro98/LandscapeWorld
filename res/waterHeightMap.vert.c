#version 120

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection; 

void main()
{
	gl_TexCoord[0] = gl_Vertex;
	gl_Position = gl_Vertex * 2.0 - 1.0;
}
