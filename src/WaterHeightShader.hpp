#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class WaterHeightShader : public Shader
{
public:
	WaterHeightShader(std::string name);
	~WaterHeightShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadwaterHeightMapDistWidth(float);
	void loadwaterHeightMapDistHeight(float);

private:
	GLuint m_location_waterHeightMapDistance_W; 
	GLuint m_location_waterHeightMapDistance_H;
};

