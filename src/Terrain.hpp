#pragma once
#include <glm/gtc/type_ptr.hpp>

#include "Model.hpp"
#include "Loader.hpp"
#include "HeightGenerator.hpp"

#define SIZE 128
#define VERTEX_COUNT 16


class Terrain
{
public:
	Terrain(int tileX, int tileZ);
	~Terrain();

	Mesh* generateTerrain();

	glm::vec3 calculateNormal(int x, int z);

	float getHeight(int x, int z);
	float lookUpHeight(int x, int z);
	float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
	float getInterpHeight(float x, float z);

	Mesh *getMesh();
	int getX();
	int getZ();

private:
	int m_tileX, m_tileZ;
	Mesh * m_mesh;
	float *m_heightMap;
};

