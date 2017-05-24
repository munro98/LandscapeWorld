#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Mesh.hpp"
#include "Texture.hpp"

class Loader
{
public:
	Loader();
	~Loader();
	static Mesh * loadToVAO(std::vector<float>& positions, std::vector<float>& textureCoords, std::vector<float>& normals, std::vector<int>& indices);
	static Mesh * loadToVAO2(std::vector<float>& positions, std::vector<float>& textureCoords, std::vector<float>& normals, std::vector<int>& indices);
	static Mesh * loadToVAO(std::vector<float>& positions, int dimensions);
	static GLuint createVAO();
	static void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float>& data);
	static void bindIndicesBuffer(std::vector<int>& indices);

	static Texture * loadTexture(int width, int height, int * pixels);

	static Texture* loadTexture(std::string fileName);

	static GLuint loadCubeMap(std::vector<std::string> cubeFaces);

};

