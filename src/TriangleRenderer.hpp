#pragma once

#include <GL/glew.h>
#include "Shader.hpp"

class TriangleRenderer
{
public:
	TriangleRenderer();
	~TriangleRenderer();

	void render();
private:

	Shader m_shader;
	GLuint m_VBO;
	GLuint m_VAO;
};

