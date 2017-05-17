#include "Model.hpp"
#include "OBJLoader.hpp"


Model::Model(const std::vector<GLfloat> vertexPositions)
{
	
	glGenBuffers(1, &m_vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexId);

	
	//glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertexPositions.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions[0]) * vertexPositions.size(), vertexPositions.data(), GL_STATIC_DRAW);
	/*
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	*/
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

Model::~Model()
{
	glDeleteBuffers(1, &m_vertexId);
}

void Model::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexId);
}

void Model::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
