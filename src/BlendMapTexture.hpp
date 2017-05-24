#pragma once
#include <iostream>
#include <GL/glew.h>

//#include "Terrain.hpp"
//#include "TerrainRenderer.hpp"

class Terrain;

class BlendMapTexture
{
public:
	BlendMapTexture(int width, int height, Terrain *t);
	~BlendMapTexture();

	int *m_pixels;
private:
	//int *m_pixels;
	int m_width, m_height;
	GLuint m_textureID;
};

