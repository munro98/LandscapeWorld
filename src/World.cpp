#include <iostream>
#include "World.hpp"


World::World() : m_isRunning(true)
{
	TerrainPosition terrainPos(0, 0);
	Terrain* terrain = new Terrain(0, 0);
	terrain->sendToGPU();
	m_terrains.insert({ terrainPos,  terrain });

	//Spawn worker pool for making terrains
	for (int i = 0; i < THREADS; ++i)
	{
		m_threads[i] = new std::thread(&World::threadUpdateTerrains, this);
	}
}

World::~World()
{
	m_isRunning = false;
	
	m_threadVariable.notify_all();

	for (int i = 0; i < THREADS; ++i)
	{
		m_threads[i]->join();
	}

	for (int i = 0; i < THREADS; ++i)
	{
		delete m_threads[i];
	}

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

	while (!m_terrainsToGenerate.empty())
	{
		m_terrainsToGenerate.pop();
	}

	
	//*/
}

//Delete all terrain and start over with new seeded value
void World::applyNewSeed(int newSeed, bool interpolateNoise) {
	m_isRunning = false;

	m_threadVariable.notify_all();

	for (int i = 0; i < THREADS; ++i)
	{
		m_threads[i]->join();
	}

	for (int i = 0; i < THREADS; ++i)
	{
		delete m_threads[i];
	}

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

	while (!m_terrainsToGenerate.empty())
	{
		m_terrainsToGenerate.pop();
	}


	
	for (auto it = m_terrainsToAddToMap.begin(); it != m_terrainsToAddToMap.end();) {
		auto toErase = it;
		++it;
		m_terrainsToAddToMap.erase(toErase);
	}

	///////////////////////// Everything is cleaned
	HeightGenerator::m_seed = newSeed;
	HeightGenerator::m_interpolateNoise = interpolateNoise;
	m_isRunning = true;

	for (int i = 0; i < THREADS; ++i)
	{
		m_threads[i] = new std::thread(&World::threadUpdateTerrains, this);
	}


}

//Queue new terrains/ delete far terrains as camera moves around the world
void World::update(float playerX, float playerZ)
{	
	///*

	int x = (int)std::floor(playerX / TERRAIN_SIZE);
	int z = (int)std::floor(playerZ / TERRAIN_SIZE);

	//std::cout << x << ", " << z << std::endl;

	updateTile(x, z);
	//Out ward spiral grid alogorithm for queued terrains
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
		updateTile(x + i, z + j);

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


	// Take 1 newly generated generated terrain and sends its data to gpu
	// and place in hash map
	m_terrainsGeneratedMutex.lock();
	if (!m_terrainsGenerated.empty())
	{
		Terrain* terrain = m_terrainsGenerated.front();
		m_terrainsGenerated.pop();
		m_terrainsGeneratedMutex.unlock();

		terrain->sendToGPU();
		TerrainPosition terrainPos(terrain->getX(), terrain->getZ());

		m_terrains.insert({ terrainPos,  terrain });

		m_terrainsToAddToMap.erase(terrainPos);

	}
	else {
		m_terrainsGeneratedMutex.unlock();

	}
	
}
// Queue terrain if not in hash map and not all ready queued
void World::updateTile(int x, int z) {
	TerrainPosition terrainPos(x, z);
	auto TileIt = m_terrains.find(terrainPos);
	auto TileSetIt = m_terrainsToAddToMap.find(terrainPos);
	if (TileIt == m_terrains.end() && TileSetIt == m_terrainsToAddToMap.end())
	{
		m_terrainsToAddToMap.insert(terrainPos);

		m_terrainsToGenerateMutex.lock();
		m_terrainsToGenerate.push(terrainPos);
		m_terrainsToGenerateMutex.unlock();

		m_threadVariable.notify_all();
	}

}

// Worker pool runs in this function and makes terrains for the main thread
void World::threadUpdateTerrains() {

	while (m_isRunning) {
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

int World::chunkVisible(Frustum & frustum, int terrainX, int terrainZ) {
	BoundingBox box(glm::vec3(0.0f, -140, 0.0f), glm::vec3(TERRAIN_SIZE, 140, TERRAIN_SIZE), glm::vec3(terrainX * TERRAIN_SIZE, 0.0f, terrainZ * TERRAIN_SIZE));
	return frustum.testIntersection(box);
}

void World::checkTerrainInFrustum(Frustum &frustum)
{
	for (auto it = m_terrains.begin(); it != m_terrains.end(); ++it) {

		Terrain *terrain = it->second;
		int terrainX = terrain->getX();
		int terrainZ = terrain->getZ();

		int vis = chunkVisible(frustum, terrainX, terrainZ);
		terrain->m_isInFrustum = vis;
		
	}
}

//Render all the terrains
void World::render()
{
	for (auto it = m_terrains.begin(); it != m_terrains.end(); ++it) {
		//int terrainX = it->second->getX();
		//int terrainZ = it->second->getZ();

		Terrain *terrain = it->second;
		if (terrain->m_isInFrustum == Frustum::TEST_INTERSECT ||terrain->m_isInFrustum == Frustum::TEST_INSIDE) {
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
}

//TODO Implement this function
//
void World::rayCastTerrain(glm::vec3& start, glm::vec3& forward)
{
	static float step = 0.1f;

	int x = (int)std::floor(start.x / TERRAIN_SIZE);
	int z = (int)std::floor(start.z / TERRAIN_SIZE);



}

//Finds terrain at specific world coordinates if it exists
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
//Finds terrain tile given its x and z grid position if it exists
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
//Returns terrain height at a given world position if terrain exists there
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

glm::vec3 World::normalAt(float worldX, float worldZ)
{
	int x = (int)std::floor(worldX / TERRAIN_SIZE);
	int z = (int)std::floor(worldZ / TERRAIN_SIZE);

	TerrainPosition terrainPos(x, z);
	auto chunkIt = m_terrains.find(terrainPos);
	if (chunkIt == m_terrains.end())
	{
		return glm::vec3(0.0f, 1.0f, 0.0f);
	}

	float terrainX = worldX - (x * TERRAIN_SIZE);
	float terrainZ = worldZ - (z * TERRAIN_SIZE);

	return chunkIt->second->getNormal(terrainX, terrainZ);
}