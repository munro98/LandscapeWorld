#include "Terrain.hpp"


Terrain::Terrain(int tileX, int tileZ) : m_tileX(tileX), m_tileZ(tileZ), m_heightMap(new float[(TERRAIN_GRID_SIZE + 3) * (TERRAIN_GRID_SIZE + 3)])
{
	//std::cout << "terrain: " << tileX << " " << tileZ << "\n";
	
	
	generateTerrain();
	m_blendMapTexture = new BlendMapTexture(512, 512, this);

	//m_mesh = Loader::loadToVAO(*m_vertices, *m_textureCoords, *m_normals, *m_indices);
	//m_blendMapTexture = new BlendMapTexture (512, 512, this);
	//m_texture = Loader::loadTexture(512, 512, m_blendMapTexture->m_pixels);

	//sendToGPU();
	//m_blendMapTexture = new BlendMapTexture(512, 512, this);
	//m_texture = Loader::loadTexture(512, 512, m_blendMapTexture->m_pixels);
	//m_mesh = Loader::loadToVAO(*m_vertices, *m_textureCoords, *m_normals, *m_indices);
}


Terrain::~Terrain()
{
	delete m_heightMap;
	delete m_mesh;
	delete m_texture;
}

void Terrain::sendToGPU() {

	
	m_texture = Loader::loadTexture(512, 512, m_blendMapTexture->m_pixels);
	m_mesh = Loader::loadToVAO(*m_vertices, *m_textureCoords, *m_normals, *m_indices);

	delete m_vertices;
	delete m_normals;
	delete m_textureCoords;
	delete m_indices;
	delete m_blendMapTexture;
}

void Terrain::generateTerrain()
{
	int count = (TERRAIN_GRID_SIZE+1) * (TERRAIN_GRID_SIZE + 1);

	//std::vector<float> vertices(count * 3);
	//std::vector<float> normals(count * 3);
	//std::vector<float> textureCoords(count * 2);
	//std::vector<int> indices(6 * (TERRAIN_GRID_SIZE)*(TERRAIN_GRID_SIZE));

	m_vertices = new std::vector<float> (count * 3);
	m_normals = new std::vector<float> (count * 3);
	m_textureCoords = new std::vector<float> (count * 2);
	m_indices = new std::vector<int> (6 * (TERRAIN_GRID_SIZE)*(TERRAIN_GRID_SIZE));

	int vertexIndex = 0;

	for (int x = 0; x < (TERRAIN_GRID_SIZE + 3); x++) {

		for (int z = 0; z < (TERRAIN_GRID_SIZE + 3); z++) {
			int i = x + z * (TERRAIN_GRID_SIZE + 3);
			float height = getHeight(x-1, z-1);
			m_heightMap[i] = height;

			//m_heightMap[i] = 0.0f;
		}
	}

	//Erode
	
	for (int x = 0; x < TERRAIN_GRID_SIZE+1; x++) {

		for (int z = 0; z < TERRAIN_GRID_SIZE+1; z++) {
			//int i = x + z * TERRAIN_GRID_SIZE;
			//int i2 = (x+1) + ((z+1) * (TERRAIN_GRID_SIZE + 2));
			//assert(i2 < ((TERRAIN_GRID_SIZE + 2) * (TERRAIN_GRID_SIZE + 2)));

			(*m_vertices)[vertexIndex * 3] = (float)(m_tileX * TERRAIN_SIZE) + ((float)x / ((float)TERRAIN_GRID_SIZE) * TERRAIN_SIZE);
			(*m_vertices)[vertexIndex * 3 + 1] = lookUpHeight(x, z);// m_heightMap[i2];
			(*m_vertices)[vertexIndex * 3 + 2] = (float)(m_tileZ * TERRAIN_SIZE) + ((float)z / ((float)TERRAIN_GRID_SIZE) * TERRAIN_SIZE);

			///*
			glm::vec3 normal = calculateNormal(x, z);
			(*m_normals)[vertexIndex * 3] = normal.x;
			(*m_normals)[vertexIndex * 3 + 1] = normal.y;
			(*m_normals)[vertexIndex * 3 + 2] = normal.z;
			//*/

			/*
			normals[vertexIndex * 3] = 0.0f;
			normals[vertexIndex * 3 + 1] = 1.0f;
			normals[vertexIndex * 3 + 2] = 0.0f;
			*/

			(*m_textureCoords)[vertexIndex * 2] = (float)x / ((float)TERRAIN_GRID_SIZE);
			(*m_textureCoords)[vertexIndex * 2 + 1] = (float)z / ((float)TERRAIN_GRID_SIZE);

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

			(*m_indices)[vertexIndex++] = topLeft;
			(*m_indices)[vertexIndex++] = topRight;
			(*m_indices)[vertexIndex++] = bottomLeft;
			(*m_indices)[vertexIndex++] = topRight;
			(*m_indices)[vertexIndex++] = bottomRight;
			(*m_indices)[vertexIndex++] = bottomLeft;

		}
	}

	/*
	vertexIndex = 0;
	int normalIndex = 0;

	for (int i = 0; i < indices.size();i+=3) {

		glm::vec3 v1(vertices[indices[i]*3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
		glm::vec3 v2(vertices[indices[i+1] * 3], vertices[indices[i+1] * 3 +1], vertices[indices[i+1] * 3 +2]);
		glm::vec3 v3(vertices[indices[i+2] * 3], vertices[indices[i+2] * 3 +1], vertices[indices[i+2] * 3 +2]);

		glm::vec3 e1(v2 - v1);
		glm::vec3 e2(v3 - v1);

		glm::vec3 normal(glm::normalize(glm::cross(e1, e2)));
		//std::cout << normal.x << " " << normal.y << " " << normal.z << "\n";
		//std::cout << v2.x << " " << v2.y << " " << v2.z << "\n";
		//std::cout << v1.x << " " << v1.y << " " << v1.z << "\n\n";

		//std::cout << e1.x << " " << e1.y << " " << e1.z << "\n";
		//std::cout << e2.x << " " << e2.y << " " << e2.z << "\n\n";

		normals[indices[i] * 3] = normal.x;
		normals[indices[i] * 3 + 1] = normal.y;
		normals[indices[i] * 3 + 2] = normal.z;

		normals[indices[i+1] * 3] = normal.x;
		normals[indices[i+1] * 3 + 1] = normal.y;
		normals[indices[i+1] * 3 + 2] = normal.z;

		normals[indices[i + 2] * 3] = normal.x;
		normals[indices[i + 2] * 3 + 1] = normal.y;
		normals[indices[i + 2] * 3 + 2] = normal.z;

	}
	*/
}

glm::vec3 Terrain::calculateNormal(int x, int z) {

	float heightL = lookUpHeight(x - 1, z);
	float heightR = lookUpHeight(x + 1, z);
	float heightD = lookUpHeight(x, z - 1);
	float heightU = lookUpHeight(x, z + 1);

	glm::vec3 normal(heightL - heightR, 8.0f, heightD - heightU);
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

glm::vec3 Terrain::getNormal(float x, float z) {
	float gridSquareSize = (float)TERRAIN_SIZE / ((float)TERRAIN_GRID_SIZE);
	int gridX = (int)std::floor(x / gridSquareSize);
	int gridZ = (int)std::floor(z / gridSquareSize);



	if (gridX >= TERRAIN_GRID_SIZE || gridZ >= TERRAIN_GRID_SIZE || gridX < 0 || gridZ < 0) {
		return glm::vec3(0,0,0);
	}

	float xCoord = ((int)x % (int)gridSquareSize) / gridSquareSize;
	float zCoord = ((int)z % (int)gridSquareSize) / gridSquareSize;
	glm::vec3 result;

	result = calculateNormal(gridX, gridZ);

	//std::cout << gridX << " " << gridZ << " , " << xCoord << " " << zCoord << " " << "\n";
	//std::cout << xCoord << " " << zCoord << " " << "\n";
	std::cout << result.x << " " << result.y << " " << result.z << "\n";

	return result;
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