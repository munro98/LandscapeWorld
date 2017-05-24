#include "TerrainRenderer.hpp"

#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

#include "BlendMapTexture.hpp"

TerrainRenderer::TerrainRenderer(glm::mat4 projectionMatrix, World &world) : m_shader(TerrainShader("terrainShader")), m_world(world)
//TerrainRenderer::TerrainRenderer(glm::mat4 projectionMatrix) : m_shader(TerrainShader("terrainShader"))
{

	m_shader.use();
	m_shader.loadProjectionMatrix(projectionMatrix);
	m_shader.loadLightPosition(glm::vec3(1.0, 100.0f, 10.0f));
	m_shader.loadLightColour(glm::vec3(1.4, 1.4, 1.4));

	m_shader.stop();

	//Generate BlendMapTexture for each Terrain
	//BlendMapTexture blendMapTexture(2048, 2048);
	//BlendMapTexture blendMapTexture(2048, 2048, );
	//m_texture = Loader::loadTexture(2048, 2048, blendMapTexture.m_pixels);

	//m_texture = Loader::loadTexture("ground2048");

}

TerrainRenderer::~TerrainRenderer()
{
}

void TerrainRenderer::render(glm::mat4 view, glm::mat4 model, glm::mat4 projection, glm::vec3 camPos)
{
	m_shader.use();
	//Update uniforms
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);
	m_shader.loadProjectionMatrix(projection);

	m_shader.loadCamPos(camPos);

	//Bind texture

	glActiveTexture(GL_TEXTURE0);
	//m_texture->bind();

	/*
	for (int i = 0; i < terrains.size(); i++) {
		//glBindVertexArray(m_terrain.getMesh()->getVaoID());
		glBindVertexArray(terrains[i]->getMesh()->getVaoID());
		//glDrawElements(GL_TRIANGLES, m_terrain.getMesh()->getVertexCount(), GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, terrains[i]->getMesh()->getVertexCount(), GL_UNSIGNED_INT, 0);
	}
	*/
	m_world.render(0,0);
	Texture::unbind();

	glBindVertexArray(0);

}