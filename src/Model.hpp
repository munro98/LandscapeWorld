#pragma once
#include <GL/glew.h>
#include <vector>

class Model
{
public:
	Model(const std::vector<GLfloat> vertexPositions);
	void bind();
	void unbind();
	~Model();
private:
	GLuint m_vertexId;
};

