#include <iostream>
#include "World.hpp"


World::World() : m_isRunning(true)
{
	TerrainPosition terrainPos(0, 0);
	Terrain* terrain = new Terrain(0, 0);
	m_terrains.insert({ terrainPos,  terrain });

	for (int i = 0; i < THREADS; ++i)
	{
		//m_threads[i] = new std::thread(&World::threadUpdateTerrains, this);
	}
}

World::~World()
{

	for (auto it = m_terrains.begin(); it != m_terrains.end();) {
		auto terrain = it->second;
		delete terrain;
		auto toErase = it;
		++it;
		m_terrains.erase(toErase);
	}
	///*
	while (!m_terrainsGenerated.empty())
	{
		delete m_terrainsGenerated.front();
		m_terrainsGenerated.pop();
	}
	//*/
}

void World::update(float playerX, float playerZ)
{	
	///*

	int x = (int)std::floor(playerX / TERRAIN_SIZE);
	int z = (int)std::floor(playerZ / TERRAIN_SIZE);

	//std::cout << x << ", " << z << std::endl;

	updateChunk(x, z);
	///*
#if 1
	int di = 1;
	int dj = 0;
	int segmentLength = 1;

	// current position (i, j) and how much of current segment we passed
	int i = 0;
	int j = 0;
	int segmentPassed = 0;
	for (int k = 0; k < WORLD_UPDATE_RADIUS * WORLD_UPDATE_RADIUS; ++k) {
		// make a step, add 'direction' vector (di, dj) to current position (i, j)
		i += di;
		j += dj;
		++segmentPassed;
		//std::cout << i << " " << j << "\n";
		updateChunk(x + i, z + j);

		if (segmentPassed == segmentLength) {
			// done with current segment
			segmentPassed = 0;

			//rotate direction
			int buffer = di;
			di = -dj;
			dj = buffer;

			// increase segment length if necessary
			if (dj == 0) {
				++segmentLength;
			}
		}
	}
#endif
	//*/
	///*
	//Remove far terrains
	for (auto it = m_terrains.begin(); it != m_terrains.end();) {
		auto terrain = it->second;
		if (abs(terrain->getX() - x) > WORLD_DELETE_RADIUS || abs(terrain->getZ() - z) > WORLD_DELETE_RADIUS) {
			delete terrain;
			auto toErase = it;
			++it;
			m_terrains.erase(toErase);
		}
		else
		{
			++it;
		}
	}
	//*/

	if (!m_terrainsToGenerate.empty())
	{
		TerrainPosition terrainPos = m_terrainsToGenerate.front();
		m_terrainsToGenerate.pop();
		Terrain* terrain = new Terrain(terrainPos.x, terrainPos.z);

		m_terrains.insert({ terrainPos,  terrain });
	}

	/* Implement if terrain deformation is added
	if(!m_chunksGenerated.empty())
	{
		Terrain* terrain = m_chunksGenerated.front();
		m_chunksGenerated.pop();
		//TerrainPosition chunkPos(terrain->getX(), terrain->getZ());
		auto chunkIt = m_terrains.find(terrainPos);

		
		Terrain* frontTerrain = findChunkAt(chunkPos.x, chunkPos.z - 1);
		Terrain* backTerrain = findChunkAt(chunkPos.x, chunkPos.z + 1);
		Terrain* leftTerrain = findChunkAt(chunkPos.x - 1, chunkPos.z);
		Terrain* rightTerrain = findChunkAt(chunkPos.x + 1, chunkPos.z);

		//terrain->updateSideFaces(frontTerrain, backTerrain, leftTerrain, rightTerrain);
		//terrain->updateMesh();

		if (frontTerrain != nullptr)
		{
		frontTerrain->updateBack(terrain);
		}

		if (backTerrain != nullptr)
		{

		backTerrain->updateFront(terrain);
		}

		if (leftTerrain != nullptr)
		{
		leftTerrain->updateRight(terrain);
		}

		if (rightTerrain != nullptr)
		{
		rightTerrain->updateLeft(terrain);
		}
		

		m_terrains.insert({ terrainPos,  terrain });
		m_chunksToAddToMap.erase(terrainPos);
	}
	*/
	
}

void World::updateChunk(int x, int z) {
	TerrainPosition terrainPos(x, z);
	auto chunkIt = m_terrains.find(terrainPos);
	auto chunkSetIt = m_terrainsToAddToMap.find(terrainPos);
	if (chunkIt == m_terrains.end() && chunkSetIt == m_terrainsToAddToMap.end())
	{
		m_terrainsToAddToMap.insert(terrainPos);
		m_terrainsToGenerate.push(terrainPos);
	}

}

void World::threadUpdateTerrains() {

	while (m_isRunning) {
		//m_ChunkSetQueue lock
		m_terrainsToGenerateMutex.lock();
		if (!m_terrainsToGenerate.empty())
		{
			TerrainPosition terrainPos = m_terrainsToGenerate.front();
			m_terrainsToGenerate.pop();
			m_terrainsToGenerateMutex.unlock();

			Terrain* terrain = new Terrain(terrainPos.x, terrainPos.z);

			m_terrainsGeneratedMutex.lock();
			m_terrainsGenerated.push(terrain);
			m_terrainsGeneratedMutex.unlock();
		}
		else
		{
			m_terrainsToGenerateMutex.unlock();
			std::unique_lock<std::mutex> lck(m_threadMutex);
			m_threadVariable.wait(lck);
		}
	}

}


void World::render(float playerX, float playerZ)
{
	for (auto it = m_terrains.begin(); it != m_terrains.end(); ++it) {
		//int terrainX = it->second->getX();
		//int terrainZ = it->second->getZ();

		Terrain *terrain = it->second;
		Mesh *mesh = terrain->getMesh();
		Texture *texture = terrain->getBlendMapTexture();
		glActiveTexture(GL_TEXTURE0);
		texture->bind();
		
		glBindVertexArray(mesh->getVaoID());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glDrawElements(GL_TRIANGLES, mesh->getVertexCount(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}

//TODO Implement this function
//
void World::rayCastTerrain(glm::vec3& start, glm::vec3& forward)
{
	static float step = 0.1f;

	int x = (int)std::floor(start.x / TERRAIN_SIZE);
	int z = (int)std::floor(start.z / TERRAIN_SIZE);



}

Terrain* World::findTerrainAt(float worldX, float worldZ)
{
	int x = (int)std::floor(worldX / TERRAIN_SIZE);
	int z = (int)std::floor(worldZ / TERRAIN_SIZE);
	TerrainPosition terrainPos(x, z);
	auto chunkIt = m_terrains.find(terrainPos);
	if (chunkIt == m_terrains.end())
	{
		return nullptr;
	}
	return chunkIt->second;
}

Terrain* World::findTerrainAt(int x , int z)
{
	TerrainPosition terrainPos(x, z);
	auto chunkIt = m_terrains.find(terrainPos);
	if (chunkIt == m_terrains.end())
	{
		return nullptr;
	}
	return chunkIt->second;
}

float World::heightAt(float worldX, float worldZ)
{
	int x = (int)std::floor(worldX / TERRAIN_SIZE);
	int z = (int)std::floor(worldZ / TERRAIN_SIZE);

	TerrainPosition terrainPos(x, z);
	auto chunkIt = m_terrains.find(terrainPos);
	if (chunkIt == m_terrains.end())
	{
		return 0;
	}

	float terrainX = worldX - (x * TERRAIN_SIZE);
	float terrainZ = worldZ - (z * TERRAIN_SIZE);
	//float height = chunkIt->second->lookUpHeight(terrainX*0.125, terrainZ*0.125);
	//float height = chunkIt->second->getInterpHeight(worldX, worldZ);
	float height = chunkIt->second->getInterpHeight(terrainX, terrainZ);
	//std::cout << terrainX << " " << terrainZ << " " << height << "\n";
	return height;
}
