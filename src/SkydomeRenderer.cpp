#include "SkydomeRenderer.hpp"
#include "OBJLoader.hpp"
#include "Shader.hpp"


SkydomeRenderer::SkydomeRenderer(glm::mat4 projectionMatrix) : m_shader(SkydomeShader("skydomeShader"))
{
	m_cube = OBJLoader::loadObjModel("skydome");

	m_shader.use();
	m_shader.loadProjectionMatrix(projectionMatrix);
	m_shader.stop();
}


SkydomeRenderer::~SkydomeRenderer()
{
}

void SkydomeRenderer::render(glm::mat4 view, glm::mat4 & model)
{
	//Sky doesn't move
	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;

	m_shader.use();
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);

	glBindVertexArray(m_cube->getVaoID());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, m_cube->getVertexCount(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
	m_shader.stop();
}