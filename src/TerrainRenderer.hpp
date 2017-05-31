#pragma once
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "TerrainShader.hpp"

#include "Terrain.hpp"
#include "World.hpp"

class TerrainRenderer
{
public:
	//TerrainRenderer(glm::mat4 projectionMatrix);
	TerrainRenderer(glm::mat4 projectionMatrix, World &world);
	~TerrainRenderer();

	void render(glm::mat4 view, glm::mat4 model, glm::mat4 projection, glm::vec3 camPos, float showBlendMap);

	void render(glm::mat4 view, glm::mat4 model, glm::mat4 projection, glm::vec3 camPos);

private:
	TerrainShader m_shader;
	Texture *m_grass;
	Texture *m_rock;
	Texture *m_stones;
	Texture *m_snow;
	
	
	World &m_world;

	//Terrain m_terrain;
};

