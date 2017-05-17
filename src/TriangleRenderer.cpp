#include "TriangleRenderer.hpp"


TriangleRenderer::TriangleRenderer() : m_shader(Shader("shader"))
{
	m_shader.use();
	//Update uniforms here
	m_shader.stop();

	// Send simple triangle to GPU
	static GLfloat vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);


	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void TriangleRenderer::render() {
	//Draw simple triangle
	m_shader.use();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	m_shader.stop();
}


TriangleRenderer::~TriangleRenderer()
{
}
