#include "WaterRenderer.hpp"
#include "WaterShader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"


WaterRenderer::WaterRenderer(glm::mat4& projectionMatrix) : m_shader(WaterShader("waterShader"))
{
	m_shader.use();
	m_shader.loadProjectionMatrix(projectionMatrix);
	m_shader.stop();

	//m_mesh = OBJLoader::loadObjModel("sphere");
	//m_texture = Loader::loadTexture("ground2048");
	m_mesh = OBJLoader::loadObjModel("waterplane");
	m_texture = Loader::loadTexture("waterNormalMap");
}

WaterRenderer::~WaterRenderer()
{
}


void WaterRenderer::render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition)
{
	m_shader.use();
	//Update uniforms
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);
	m_shader.loadProjectionMatrix(projection);
	m_shader.loadCameraPosition(cameraPosition);

	glBindVertexArray(m_mesh->getVaoID());


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	m_texture->bind();

	glDrawElements(GL_TRIANGLES, m_mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
	Texture::unbind();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//*/
	glBindVertexArray(0);


}