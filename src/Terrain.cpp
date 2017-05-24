#include "Terrain.hpp"


Terrain::Terrain(int tileX, int tileZ) : m_tileX(tileX), m_tileZ(tileZ), m_heightMap(new float[(TERRAIN_GRID_SIZE + 3) * (TERRAIN_GRID_SIZE + 3)])
{
	std::cout << "terrain: " << tileX << " " << tileZ << "\n";
	m_mesh = generateTerrain();


	BlendMapTexture blendMapTexture(512, 512, this);
	m_texture = Loader::loadTexture(512, 512, blendMapTexture.m_pixels);
	//m_texture = Loader::loadTexture("ground2048");
}


Terrain::~Terrain()
{
	delete m_heightMap;
	delete m_mesh;
	delete m_texture;
}

Mesh * Terrain::generateTerrain()
{
	int count = (TERRAIN_GRID_SIZE+1) * (TERRAIN_GRID_SIZE + 1);

	//TODO use fixed size arrays.
	std::vector<float> vertices = { -0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f };
	std::vector<float> normals = { 0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f };
	std::vector<float> textureCoords = { -0.5f,-0.5f,
		0.5f,-0.5f,
		0.0f,0.5f};
	std::vector<int> indices = {0, 1, 2};
	
	int vertexIndex = 0;

	///*
	vertices.resize(count * 3);
	normals.resize(count * 3);
	textureCoords.resize(count * 2);


	indices.resize(6 * (TERRAIN_GRID_SIZE)*(TERRAIN_GRID_SIZE));

	for (int x = 0; x < (TERRAIN_GRID_SIZE + 3); x++) {

		for (int z = 0; z < (TERRAIN_GRID_SIZE + 3); z++) {
			int i = x + z * (TERRAIN_GRID_SIZE + 3);
			int height = getHeight(x-1, z-1);
			m_heightMap[i] = height;
			//heightMap[i] = 0.0f;
		}
	}

	//Erode
	
	for (int x = 0; x < TERRAIN_GRID_SIZE+1; x++) {

		for (int z = 0; z < TERRAIN_GRID_SIZE+1; z++) {
			//int i = x + z * TERRAIN_GRID_SIZE;
			//int i2 = (x+1) + ((z+1) * (TERRAIN_GRID_SIZE + 2));
			//assert(i2 < ((TERRAIN_GRID_SIZE + 2) * (TERRAIN_GRID_SIZE + 2)));

			vertices[vertexIndex * 3] = (float)(m_tileX * TERRAIN_SIZE) + ((float)x / ((float)TERRAIN_GRID_SIZE) * TERRAIN_SIZE);
			vertices[vertexIndex * 3 + 1] = lookUpHeight(x, z);// m_heightMap[i2];
			vertices[vertexIndex * 3 + 2] = (float)(m_tileZ * TERRAIN_SIZE) + ((float)z / ((float)TERRAIN_GRID_SIZE) * TERRAIN_SIZE);

			///*
			glm::vec3 normal = calculateNormal(x, z);
			normals[vertexIndex * 3] = normal.x;
			normals[vertexIndex * 3 + 1] = normal.y;
			normals[vertexIndex * 3 + 2] = normal.z;
			//*/

			/*
			normals[vertexIndex * 3] = 0.0f;
			normals[vertexIndex * 3 + 1] = 1.0f;
			normals[vertexIndex * 3 + 2] = 0.0f;
			*/

			textureCoords[vertexIndex * 2] = (float)x / ((float)TERRAIN_GRID_SIZE);
			textureCoords[vertexIndex * 2 + 1] = (float)z / ((float)TERRAIN_GRID_SIZE);

			vertexIndex++;
		}

	}

	vertexIndex = 0;
	for (int z = 0; z < TERRAIN_GRID_SIZE; z++) {
		for (int x = 0; x < TERRAIN_GRID_SIZE; x++) {
			int topLeft = (z * (TERRAIN_GRID_SIZE+1)) + x;
			int topRight = topLeft + 1;

			int bottomLeft = ((z + 1) * (TERRAIN_GRID_SIZE+1)) + x;
			int bottomRight = bottomLeft + 1;

			indices[vertexIndex++] = topLeft;
			indices[vertexIndex++] = topRight;
			indices[vertexIndex++] = bottomLeft;
			indices[vertexIndex++] = topRight;
			indices[vertexIndex++] = bottomRight;
			indices[vertexIndex++] = bottomLeft;

		}
	}
	

	Mesh *mesh = Loader::loadToVAO(vertices, textureCoords, normals, indices);
	return mesh;
}

glm::vec3 Terrain::calculateNormal(int x, int z) {

	float heightL = lookUpHeight(x - 1, z);
	float heightR = lookUpHeight(x + 1, z);
	float heightD = lookUpHeight(x, z - 1);
	float heightU = lookUpHeight(x, z + 1);

	glm::vec3 normal(heightL - heightR, 4.0f, heightD - heightU);
	return normal;
}

float Terrain::getHeight(int x, int z) {
	//std::cout << "getHieght: " << (m_tileX * TERRAIN_GRID_SIZE) + x << " " << (m_tileZ * TERRAIN_GRID_SIZE) + z << "\n";
	//return HeightGenerator::generateHeight((m_tileX * TERRAIN_GRID_SIZE) + x - m_tileX, (m_tileZ * TERRAIN_GRID_SIZE) + z - m_tileZ);
	return HeightGenerator::generateHeight((m_tileX * TERRAIN_GRID_SIZE) + x, (m_tileZ * TERRAIN_GRID_SIZE) + z);
}

float Terrain::lookUpHeight(int x, int z) {
	int i = (x + 1) + ((z + 1) * (TERRAIN_GRID_SIZE + 3));
	return m_heightMap[i];
}

float Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

//TODO it Works!
float Terrain::getInterpHeight(float x, float z) {
	float gridSquareSize = (float)TERRAIN_SIZE / ((float)TERRAIN_GRID_SIZE);
	int gridX = (int)std::floor(x / gridSquareSize);
	int gridZ = (int)std::floor(z / gridSquareSize);

	

	if (gridX >= TERRAIN_GRID_SIZE || gridZ >= TERRAIN_GRID_SIZE || gridX < 0 || gridZ < 0) {
		return 0;
	}

	float xCoord = ((int)x % (int)gridSquareSize) / gridSquareSize;
	float zCoord = ((int)z % (int)gridSquareSize) / gridSquareSize;
	float result;

	//std::cout << gridX << " " << gridZ << " , " << xCoord << " " << zCoord << " " << "\n";
	//std::cout << xCoord << " " << zCoord << " " << "\n";

	if (xCoord <= (1 - zCoord)) {

		result = barryCentric(glm::vec3(0, lookUpHeight(gridX, gridZ), 0),
			glm::vec3(1, lookUpHeight(gridX +1, gridZ), 0),
			glm::vec3(0, lookUpHeight(gridX, gridZ +1), 1),
			glm::vec2(xCoord, zCoord));
	}
	else {

		result = barryCentric(glm::vec3(1, lookUpHeight(gridX + 1, gridZ), 0),
			glm::vec3(1, lookUpHeight(gridX + 1, gridZ +1), 1),
			glm::vec3(0, lookUpHeight(gridX, gridZ + 1), 1),
			glm::vec2(xCoord, zCoord));
	}

	return result;
}

Mesh * Terrain::getMesh()
{
	return m_mesh;
}

Texture * Terrain::getBlendMapTexture()
{
	return m_texture;
}

int Terrain::getX()
{
	return m_tileX;
}

int Terrain::getZ()
{
	return m_tileZ;
}