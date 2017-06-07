#pragma once

#include <GL/glew.h>
#include "Shader.hpp"

class GrassRenderer
{
public:
	GrassRenderer();
	~GrassRenderer();

	void render();
private:

	Shader m_shader;
	GLuint m_VBO;
	GLuint m_VAO;
};
 
