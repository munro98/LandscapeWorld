#include "ModelRenderer.hpp"
#include "ModelShader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"


ModelRenderer::ModelRenderer(glm::mat4 projectionMatrix) : m_shader(ModelShader("modelShader"))
{
	m_shader.use();
	m_shader.loadProjectionMatrix(projectionMatrix);
	m_shader.stop();

	//m_mesh = OBJLoader::loadObjModel("sphere");
	//m_texture = Loader::loadTexture("ground2048");
	m_mesh = OBJLoader::loadObjModel("box");
	m_texture = Loader::loadTexture("wood2");
}

ModelRenderer::~ModelRenderer()
{
}


void ModelRenderer::render(glm::mat4 view, glm::mat4 model, glm::mat4 projection, Mesh* box)
{
	m_shader.use();
	//Update uniforms
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);
	m_shader.loadProjectionMatrix(projection);

	glBindVertexArray(box->getVaoID());


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	m_texture->bind();

	glDrawElements(GL_TRIANGLES, box->getVertexCount(), GL_UNSIGNED_INT, 0);
	Texture::unbind();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//*/
	glBindVertexArray(0);


}