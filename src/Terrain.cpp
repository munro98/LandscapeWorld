#include "Terrain.hpp"



Terrain::Terrain(int tileX, int tileZ) : m_tileX(tileX), m_tileZ(tileZ), m_heightMap(new float[(VERTEX_COUNT + 2) * (VERTEX_COUNT + 2)])
{
	std::cout << "terrain: " << tileX << " " << tileZ << "\n";
	m_mesh = generateTerrain();
}


Terrain::~Terrain()
{
	delete m_heightMap;
	delete m_mesh;
}

Mesh * Terrain::generateTerrain()
{


	//std::vector<float> heights();

	int count = VERTEX_COUNT * VERTEX_COUNT;

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


	indices.resize(6 * (VERTEX_COUNT -1)*(VERTEX_COUNT -1));

	for (int x = 0; x < (VERTEX_COUNT + 2); x++) {

		for (int z = 0; z < (VERTEX_COUNT + 2); z++) {
			int i = x + z * (VERTEX_COUNT + 2);
			//int height = HeightGenerator::generateHeight(m_tileX * SIZE + j, m_tileZ * SIZE + i);
			//int height = HeightGenerator::generateHeight(m_tileX * (VERTEX_COUNT-1) + x, m_tileZ * (VERTEX_COUNT-1) + z);
			//int height = HeightGenerator::generateHeight((m_tileX * SIZE) + x, (m_tileZ * SIZE) + z);
			int height = getHeight(x-1, z-1);
			m_heightMap[i] = height;
			//heightMap[i] = 5.0f;
		}
	}

	//Erode

	
	for (int x = 0; x < VERTEX_COUNT; x++) {

		for (int z = 0; z < VERTEX_COUNT; z++) {
			int i = x + z * VERTEX_COUNT;

			int i2 = (x+1) + ((z+1) * (VERTEX_COUNT + 2));
			//assert(i2 < ((VERTEX_COUNT + 2) * (VERTEX_COUNT + 2)));

			vertices[vertexIndex * 3] = (float)(m_tileX * SIZE) + ((float)x / ((float)VERTEX_COUNT - 1.0f) * SIZE);
			vertices[vertexIndex * 3 + 1] = m_heightMap[i2];
			vertices[vertexIndex * 3 + 2] = (float)(m_tileZ * SIZE) + ((float)z / ((float)VERTEX_COUNT - 1.0f) * SIZE);

			//vertices[vertexIndex * 3] = (float)(m_tileX * VERTEX_COUNT) + (float)x - ((float)m_tileX * 1.0f);
			//vertices[vertexIndex * 3 + 1] = heightMap[i];
			//vertices[vertexIndex * 3 + 2] = (float)(m_tileZ * VERTEX_COUNT) + (float)z;

			
			glm::vec3 normal = calculateNormal(x, z);

			normals[vertexIndex * 3] = normal.x;
			normals[vertexIndex * 3 + 1] = normal.y;
			normals[vertexIndex * 3 + 2] = normal.z;

			
			//normals[vertexIndex * 3] = 0.0f;
			//normals[vertexIndex * 3 + 1] = 1.0f;
			//normals[vertexIndex * 3 + 2] = 0.0f;


			textureCoords[vertexIndex * 2] = (float)x / ((float)VERTEX_COUNT - 1.0f);
			textureCoords[vertexIndex * 2 + 1] = (float)z / ((float)VERTEX_COUNT - 1.0f);

			vertexIndex++;
		}

	}

	vertexIndex = 0;
	for (int z = 0; z < VERTEX_COUNT -1; z++) {
		for (int x = 0; x < VERTEX_COUNT - 1; x++) {
			int topLeft = (z * VERTEX_COUNT) + x;
			int topRight = topLeft + 1;

			int bottomLeft = ((z + 1) * VERTEX_COUNT) + x;
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
	//return HeightGenerator::generateHeight((m_tileX * SIZE) + x, (m_tileZ * SIZE) + z);
	//return HeightGenerator::generateHeight(x, z);
	//std::cout << "getHieght: " << (m_tileX * VERTEX_COUNT) + x << " " << (m_tileZ * VERTEX_COUNT) + z << "\n";
	return HeightGenerator::generateHeight((m_tileX * VERTEX_COUNT) + x - m_tileX, (m_tileZ * VERTEX_COUNT) + z - m_tileZ);
}

float Terrain::lookUpHeight(int x, int z) {
	int i = (x + 1) + ((z + 1) * (VERTEX_COUNT + 2));
	return m_heightMap[i];
}

float Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

float Terrain::getInterpHeight(float x, float z) {
	float gridSquareSize = SIZE / ((float)VERTEX_COUNT - 1.0f);
	int gridX = (int)std::floor(x / gridSquareSize);
	int gridZ = (int)std::floor(z / gridSquareSize);

	if (gridX >= VERTEX_COUNT - 1 || gridZ >= VERTEX_COUNT - 1 || gridX < 0 || gridZ < 0) {
		return 0;
	}

	float xCoord = ((int)x % (int)gridSquareSize) / gridSquareSize;
	float zCoord = ((int)z % (int)gridSquareSize) / gridSquareSize;
	float result;

	//heights[gridX][gridZ]
	if (xCoord <= (1 - zCoord)) {
		int i1 = (x + 1) + ((z + 1) * (VERTEX_COUNT + 2));
		int i2 = (x + 1 + 1) + ((z + 1) * (VERTEX_COUNT + 2));
		int i3 = (x + 1) + ((z + 1 + 1) * (VERTEX_COUNT + 2));

		result = barryCentric(glm::vec3(0, m_heightMap[i1], 0), 
			glm::vec3(1,m_heightMap[i2], 0), 
			glm::vec3(0,m_heightMap[i2], 1), 
			glm::vec2(xCoord, zCoord));
	}
	else {
		int i1 = (x + 1 + 1) + ((z + 1) * (VERTEX_COUNT + 2));
		int i2 = (x + 1 + 1) + ((z + 1 + 1) * (VERTEX_COUNT + 2));
		int i3 = (x + 1) + ((z + 1 + 1) * (VERTEX_COUNT + 2));

		result = barryCentric(glm::vec3(1, m_heightMap[i1], 0), 
			glm::vec3(1,m_heightMap[i2], 1), 
			glm::vec3(0,m_heightMap[i3], 1), 
			glm::vec2(xCoord, zCoord));
	}

	return result;
}

Mesh * Terrain::getMesh()
{
	return m_mesh;
}

int Terrain::getX()
{
	return m_tileX;
}

int Terrain::getZ()
{
	return m_tileZ;
}