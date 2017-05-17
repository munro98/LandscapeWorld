#version 330 core
in vec2 TexCoord;
in vec3 surfaceNormal;

out vec4 color;

uniform sampler2D ourTexture1;

void main()
{
	color = texture(ourTexture1, TexCoord);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}