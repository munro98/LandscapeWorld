#pragma once
#include <glm/gtc/type_ptr.hpp>

#include "Model.hpp"
#include "Loader.hpp"
#include "HeightGenerator.hpp"
#include "BlendMapTexture.hpp"

#define TERRAIN_SIZE 128
#define TERRAIN_GRID_SIZE 32


class Terrain
{
public:
	Terrain(int tileX, int tileZ);
	~Terrain();

	void sendToGPU();

	Mesh* generateTerrain();

	glm::vec3 calculateNormal(int x, int z);

	float getHeight(int x, int z);
	float lookUpHeight(int x, int z);
	float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
	float getInterpHeight(float x, float z);

	Mesh *getMesh();
	Texture *getBlendMapTexture();
	int getX();
	int getZ();

private:
	int m_tileX, m_tileZ;
	Mesh * m_mesh;
	Texture *m_texture;
	float *m_heightMap;
};

