#include "TerrainRenderer.hpp"

#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

#include "BlendMapTexture.hpp"

TerrainRenderer::TerrainRenderer(glm::mat4 projectionMatrix, World &world) : m_shader(TerrainShader("terrainShader")), m_world(world)
{

	m_shader.use();
	m_shader.loadProjectionMatrix(projectionMatrix);
	m_shader.loadLightPosition(glm::vec3(1.0, 100.0f, 10.0f));
	m_shader.loadLightColour(glm::vec3(1.4, 1.4, 1.4));

	m_shader.stop();

	//m_grass = Loader::loadTexture("ground2048");
	//m_rock = Loader::loadTexture("rock");
	//m_stones = Loader::loadTexture("rock");
	//m_snow = Loader::loadTexture("rock");

}

TerrainRenderer::~TerrainRenderer()
{
	//delete m_grass;
	//delete m_rock;
	//delete m_stones;
	//delete m_snow;
}

void TerrainRenderer::render(glm::mat4 view, glm::mat4 model, glm::mat4 projection, glm::vec3 camPos)
{
	m_shader.use();
	//Update uniforms
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);
	m_shader.loadProjectionMatrix(projection);

	m_shader.loadCamPos(camPos);

	//Bind textures

	//glActiveTexture(GL_TEXTURE1);
	//m_grass->bind();
	//glActiveTexture(GL_TEXTURE2);
	//m_rock->bind();
	//glActiveTexture(GL_TEXTURE3);
	//m_stones->bind();
	//glActiveTexture(GL_TEXTURE4);
	//m_snow->bind();

	m_world.render(0,0);
	Texture::unbind();

	glBindVertexArray(0);

}