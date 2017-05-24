#include "Loader.hpp"
#include "stb_image.h"

Loader::Loader()
{
}


Loader::~Loader()
{
}

Mesh* Loader::loadToVAO(std::vector<float>& positions, std::vector<float>& textureCoords, std::vector<float>& normals, std::vector<int>& indices)
{
	std::cout << positions.size() << " " << textureCoords.size() << " " << normals.size() << "\n";

	GLuint vaoID = createVAO();
	bindIndicesBuffer(indices);
	storeDataInAttributeList(0, 3, positions);
	storeDataInAttributeList(1, 2, textureCoords);
	storeDataInAttributeList(2, 3, normals);

	glBindVertexArray(0);

	return new Mesh(vaoID, indices.size());
}

Mesh* Loader::loadToVAO2(std::vector<float>& positions, std::vector<float>& textureCoords, std::vector<float>& normals, std::vector<int>& indices)
{
	std::cout << positions.size() << " " << textureCoords.size() << " " << normals.size() << "\n";

	std::vector<float> modelData;

	for (int vi = 0, ti = 0, ni = 0; vi < positions.size(); )
	{
		modelData.push_back(positions[vi++]);
		modelData.push_back(positions[vi++]);
		modelData.push_back(positions[vi++]);

		modelData.push_back(normals[ni++]);
		modelData.push_back(normals[ni++]);
		modelData.push_back(normals[ni++]);

		modelData.push_back(textureCoords[ti++]);
		modelData.push_back(textureCoords[ti++]);
	}

	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//Copy Indece array to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vaoID);
	//Copy Chunk data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * modelData.size(), modelData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	return new Mesh(vaoID, indices.size());
}

Mesh* Loader::loadToVAO(std::vector<float>& positions, int dimensions)
{
	GLuint vaoID = createVAO();
	storeDataInAttributeList(0, dimensions, positions);

	glBindVertexArray(0);

	return new Mesh(vaoID, positions.size()/dimensions);
}

GLuint Loader::createVAO() {

	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float>& data)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::bindIndicesBuffer(std::vector<int>& indices)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

Texture* Loader::loadTexture(int width, int height, int* pixels)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.6f);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return new Texture(width, height, textureID);

}

//TODO: Handle errors
Texture* Loader::loadTexture(std::string fileName)
{
	fileName = "./res/" + fileName + ".png";
	//std::cout << fileName;
	const char *textureName = fileName.c_str();
	//char *textureName = "./res/TextureMap.png";
	int x, y, bytesPerPixel;
	unsigned char* textureData = stbi_load(textureName, &x, &y, &bytesPerPixel, 3);

	//if (textureData)
	{
		//std::cout << "Texture load fail\n";
		//return nullptr;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.6f);


	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	return new Texture(x, y, textureID);

}

//TODO: Handle errors
GLuint Loader::loadCubeMap(std::vector<std::string> cubeFaces) {

	GLuint texID;
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	for (int i = 0; i < cubeFaces.size(); i++) {

		std::string fileName = "./res/" + cubeFaces[i] + ".png";
		const char *textureName = fileName.c_str();
		int x, y, bytesPerPixel;

		unsigned char* textureData = stbi_load(textureName, &x, &y, &bytesPerPixel, 3);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		stbi_image_free(textureData);

	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return texID;
}