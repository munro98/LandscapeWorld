#pragma once
#include <iostream>
#include <GL/glew.h>

class Terrain;

class BlendMapTexture
{
public:
	BlendMapTexture(int width, int height, Terrain *t);
	~BlendMapTexture();

	int *m_pixels;
private:
	int m_width, m_height;
	GLuint m_textureID;
};

