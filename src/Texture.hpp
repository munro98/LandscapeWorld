#pragma once

#include <GL/glew.h>

class Texture
{
public:
	Texture(int width, int height, GLuint textureID);
	~Texture();

	void bind();
	static void unbind();

private:
	int m_width, m_height;
	GLuint m_textureID;
};

