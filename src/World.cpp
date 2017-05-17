#include <iostream>
#include "World.hpp"


World::World() : m_isRunning(true)
{
	TerrainPosition terrainPos(0, 0);
	Terrain* terrain = new Terrain(0, 0);
	m_terrains.insert({ terrainPos,  terrain });
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
	/*
	while (!m_chunksGenerated.empty())
	{
		delete m_chunksGenerated.front();
		m_chunksGenerated.pop();
	}
	*/
}

void World::update(float playerX, float playerZ)
{	
	///*

	int x = (int)std::floor(playerX / SIZE);
	int z = (int)std::floor(playerZ / SIZE);

	//std::cout << x << ", " << z << std::endl;

	//updateChunk(x, z);
	/*
	int di = 1;
	int dj = 0;
	int segmentLength = 1;

	// current position (i, j) and how much of current segment we passed
	int i = 0;
	int j = 0;
	int segmentPassed = 0;
	for (int k = 0; k < MAP_UPDATE_RADIUS * MAP_UPDATE_RADIUS; ++k) {
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
	*/
	///*
	//Remove far chunks
	for (auto it = m_terrains.begin(); it != m_terrains.end();) {
		auto terrain = it->second;
		if (abs(terrain->getX() - x) > MAP_DELETE_RADIUS || abs(terrain->getZ() - z) > MAP_DELETE_RADIUS) {
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

	if (!m_chunksToGenerate.empty())
	{
		//long long int terrainPos = m_chunksToGenerate.front();
		TerrainPosition terrainPos = m_chunksToGenerate.front();
		m_chunksToGenerate.pop();
		int unpacked[2];
		//TerrainPosition::unpack(terrainPos, &unpacked[0], &unpacked[1]);
		Terrain* terrain = new Terrain(terrainPos.x, terrainPos.z);
		//Terrain* terrain = new Terrain(unpacked[0], unpacked[1]);

		m_terrains.insert({ terrainPos,  terrain });
	}

	/*
	if(!m_chunksGenerated.empty())
	{
		Terrain* terrain = m_chunksGenerated.front();
		m_chunksGenerated.pop();
		//TerrainPosition chunkPos(terrain->getX(), terrain->getZ());
		long long int terrainPos = TerrainPosition::pack(terrain->getX(), terrain->getZ());
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
	//long long int terrainPos = TerrainPosition::pack(x, z);
	TerrainPosition terrainPos(x, z);
	auto chunkIt = m_terrains.find(terrainPos);
	auto chunkSetIt = m_chunksToAddToMap.find(terrainPos);
	if (chunkIt == m_terrains.end() && chunkSetIt == m_chunksToAddToMap.end())
	{
		m_chunksToAddToMap.insert(terrainPos);
		m_chunksToGenerate.push(terrainPos);
	}

}


void World::render(float playerX, float playerZ)
{
	for (auto it = m_terrains.begin(); it != m_terrains.end(); ++it) {
		//std::cout << "rendering" << "\n";
		//int chunkX = it->second->getChunkX();
		//int chunkZ = it->second->getChunkZ();

		Terrain *terrain = it->second;
		Mesh *mesh = terrain->getMesh();
		
		glBindVertexArray(mesh->getVaoID());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glDrawElements(GL_TRIANGLES, mesh->getVertexCount(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		//it->second->render();

	}
}

void World::rayCastTerrain(glm::vec3& start, glm::vec3& forward)
{
	int x = (int)std::floor(start.x / SIZE);
	int z = (int)std::floor(start.z / SIZE);


}

Terrain* World::findTerrainAt(int x , int z)
{
	//long long int terrainPos = TerrainPosition::pack(x, z);
	TerrainPosition terrainPos(x, z);
	auto chunkIt = m_terrains.find(terrainPos);
	if (chunkIt == m_terrains.end())
	{
		return nullptr;
	}
	return chunkIt->second;
}