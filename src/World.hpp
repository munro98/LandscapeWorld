#pragma once

#define WORLD_UPDATE_RADIUS 12
#define WORLD_DELETE_RADIUS 16
#define THREADS 2

#include <mutex>

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
	void threadUpdateTerrains();
	void render(float x, float y);
	void rayCastTerrain(glm::vec3 & start, glm::vec3 & forward);
	Terrain * findTerrainAt(float worldX, float worldZ);
	Terrain* findTerrainAt(int x, int z);

	float heightAt(float worldX, float worldZ);

private:
	
	std::unordered_map<TerrainPosition,  Terrain*> m_terrains;
	std::unordered_set<TerrainPosition> m_terrainsToAddToMap;
	std::queue<TerrainPosition> m_terrainsToGenerate;
	std::queue<Terrain*> m_terrainsGenerated;


	std::mutex m_terrainsGeneratedMutex;
	std::mutex m_terrainsToGenerateMutex;
	std::mutex m_threadMutex;
	std::condition_variable m_threadVariable;

	std::mutex m_threadEndMutex;

	std::thread *m_threads[THREADS];

	bool m_isRunning;
};
