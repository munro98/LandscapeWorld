#pragma once
#include <GL/glew.h>

class Mesh
{
public:
	Mesh(GLuint vaoID, int vertexCount);
	~Mesh();

	int getVertexCount();

	GLuint getVaoID();

private:
	GLuint m_vaoID;
	int m_vertexCount;
};

