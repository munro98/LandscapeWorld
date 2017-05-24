#pragma once

#define WORLD_UPDATE_RADIUS 4
#define WORLD_DELETE_RADIUS 8

#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Terrain.hpp"
#include "TerrainPosition.hpp"

class World
{
public:
	World();
	~World();

	void update(float playerX, float playerZ);
	void updateChunk(int x, int z);
	void render(float x, float y);
	void rayCastTerrain(glm::vec3 & start, glm::vec3 & forward);
	Terrain * findTerrainAt(float worldX, float worldZ);
	Terrain* findTerrainAt(int x, int z);

	float heightAt(float worldX, float worldZ);

private:
	
	std::unordered_map<TerrainPosition,  Terrain*> m_terrains;
	std::unordered_set<TerrainPosition> m_chunksToAddToMap;
	std::queue<TerrainPosition> m_chunksToGenerate;

	bool m_isRunning;
};
