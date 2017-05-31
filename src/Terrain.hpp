#pragma once
#include <glm/gtc/type_ptr.hpp>

#include "Model.hpp"
#include "Loader.hpp"
#include "HeightGenerator.hpp"
#include "BlendMapTexture.hpp"

#define TERRAIN_SIZE 128
#define TERRAIN_GRID_SIZE 32

#define TERRAIN_GRID_ELEMENTS (TERRAIN_GRID_SIZE * TERRAIN_GRID_SIZE)

#define HEIGHTMAP_SIZE (TERRAIN_GRID_SIZE+1)

#define HEIGHTMAP_ELEMENTS (HEIGHTMAP_SIZE * HEIGHTMAP_SIZE)

#define VERTICES_ELEMENTS (HEIGHTMAP_ELEMENTS * 3)
#define INDICES_ELEMENTS (6 * (TERRAIN_GRID_SIZE)*(TERRAIN_GRID_SIZE))


class Terrain
{
public:
	Terrain(int tileX, int tileZ);
	~Terrain();

	void sendToGPU();

	void generateTerrain();

	glm::vec3 calculateNormal(int x, int z);

	float getHeight(int x, int z);
	float lookUpHeight(int x, int z);
	float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
	glm::vec3 getNormal(float x, float z);
	float getInterpHeight(float x, float z);

	Mesh *getMesh();
	Texture *getBlendMapTexture();
	int getX();
	int getZ();

	int m_isInFrustum;

private:
	int m_tileX, m_tileZ;

	Mesh * m_mesh;
	Texture *m_texture;
	float *m_heightMap;

	BlendMapTexture *m_blendMapTexture;

	std::vector<float> *m_vertices;
	std::vector<float> *m_normals;
	std::vector<float> *m_textureCoords;
	std::vector<int> *m_indices;
    bool m_sentToGPU;
};

