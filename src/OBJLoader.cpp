
#include <string>
#include <vector>


#include "OBJLoader.hpp"

#include "Loader.hpp"
#include "Mesh.hpp"


OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}


Mesh* OBJLoader::loadObjModel(const std::string& fileName)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<int> indices;


	std::vector<float> normalsArray;
	std::vector<float> texturesArray;

	std::vector<float> verticesArray;

	std::ifstream file;
	file.open(("./LandscapeWorld/res/" + fileName + ".obj").c_str());

	std::string output;

	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);

			output.append(line + "\n");

			std::string comment("#");
			std::string v("v");
			std::string vt("vt");
			std::string vn("vn");
			std::string f("f");

			std::stringstream lineSS(line);
			std::string s;
			lineSS >> s;

			if (s.compare(0, comment.length(), comment) == 0)
				continue;

			
			if (s.compare(0, vt.length(), vt) == 0)
			{

				float x, y;
				lineSS >> x >> y;

				textures.push_back(glm::vec2(x, y));

				//std::cout << x << " " << y << " \n";
			}
			else if (s.compare(0, vn.length(), vn) == 0)
			{
				float x, y, z;
				lineSS >> x >> y >> z;

				normals.push_back(glm::vec3(x, y, z));
				//std::cout << s << " \n";
			} 
			else if (s.compare(0, v.length(), v) == 0)
			{

				//lineSS >> c;
				float x, y, z;
				lineSS >> x >> y >> z;

				vertices.push_back(glm::vec3(x, y, z));
				//std::cout << x << " " << y << " " << z << " \n";
			}
			else if (s.compare(0, f.length(), f) == 0)
			{

				texturesArray.resize(vertices.size() * 2);
				normalsArray.resize(vertices.size() * 3);


				//std::cout << "breaking!" << "\n";
				break;
			}
		}

		std::cout << vertices.size() << " " << textures.size() << " " << normals.size() << " " << indices.size() << " \n";

		while (file.good()) {
			output.append(line + "\n");
			std::stringstream lineSS(line);

			char c;
			lineSS >> c;

			if (c != 'f')
			{
				getline(file, line);
				continue;
			}

			for(int i = 0; i < 3; i++)
			{
				int vi11 = 0, vi12 = 0, vi13 = 0;
				lineSS >> vi11;
				c = lineSS.peek();
				if(c == '/')
				{
					// ignore the '/' character
					lineSS.ignore(1);
					c = lineSS.peek();
					if(c != '/')
					{
						lineSS >> vi12;
					}
						
					c = lineSS.peek();
					if (c == '/')
					{
						// ignore the '/' character
						lineSS.ignore(1);
						lineSS >> vi13;
					}
				}
				processVertex(vi11, vi12, vi13, indices, textures, normals, texturesArray, normalsArray);
			}

			getline(file, line);
		}
	}
	else {
		std::cerr << "File Load Fail " << fileName << std::endl;
	}

	file.close();
	
	verticesArray.resize(vertices.size() * 3);

	
	int vertexIndex = 0;

	for (glm::vec3 vertex : vertices)
	{
		verticesArray[vertexIndex++] = vertex.x;
		verticesArray[vertexIndex++] = vertex.y;
		verticesArray[vertexIndex++] = vertex.z;
	}

	//std::cout << verticesArray.size() << " " << texturesArray.size() << " " << normalsArray.size() << " " << indices.size() <<  " \n";


	Mesh *mesh = Loader::loadToVAO(verticesArray, texturesArray, normalsArray, indices);
	return mesh;
}

void OBJLoader::processVertex(int vi1, int vi2, int vi3, std::vector<int>& indices, std::vector<glm::vec2>& textures, std::vector<glm::vec3>& normals, std::vector<float>& texturesArray, std::vector<float>& normalsArray)
{
	int currentVertexIndex = vi1 - 1;
	indices.push_back(currentVertexIndex);

	if (vi2 > 0)
	{
		glm::vec2 currentTex = textures[vi2 - 1];


		texturesArray[currentVertexIndex * 2] = currentTex.x;
		texturesArray[currentVertexIndex * 2 + 1] = 1 - currentTex.y;
	}

	if (vi3 > 0)
	{
		glm::vec3 currentNorm = normals[vi3 - 1];

		normalsArray[currentVertexIndex * 3] = currentNorm.x;
		normalsArray[currentVertexIndex * 3 + 1] = currentNorm.y;
		normalsArray[currentVertexIndex * 3 + 2] = currentNorm.z;
	}
}