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

	m_shader.loadShowBlendMap(0.5f);


	m_shader.stop();
//For fast loading
#if 1
	m_grass = Loader::loadTexture("ground2048");
	m_rock = Loader::loadTexture("rock");
	m_stones = Loader::loadTexture("stones");
	m_snow = Loader::loadTexture("snow");
#else
	m_grass = Loader::loadTexture("white");
	m_rock = Loader::loadTexture("white");
	m_stones = Loader::loadTexture("white");
	m_snow = Loader::loadTexture("white");
#endif

}

TerrainRenderer::~TerrainRenderer()
{
	delete m_grass;
	delete m_rock;
	delete m_stones;
	delete m_snow;
}

void TerrainRenderer::render(glm::mat4 view, glm::mat4 model, glm::mat4 projection, glm::vec3 camPos, float showBlendMap, float snowCoverage)
{
	m_shader.use();
	//Update uniforms
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);
	m_shader.loadProjectionMatrix(projection);

	m_shader.loadCamPos(camPos);
	m_shader.loadShowBlendMap(showBlendMap);
	m_shader.loadSnowCoverage(snowCoverage);

	m_shader.loadTextures();

	//Bind textures
	//glActiveTexture(GL_TEXTURE0);
	//m_grass->bind();

	glActiveTexture(GL_TEXTURE1);
	m_grass->bind();
	glActiveTexture(GL_TEXTURE2);
	m_rock->bind();
	glActiveTexture(GL_TEXTURE3);
	m_stones->bind();
	glActiveTexture(GL_TEXTURE4);
	m_snow->bind();

	m_world.render(0,0);
	Texture::unbind();

	glBindVertexArray(0);

}
