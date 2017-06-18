#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class WaterHeightShader : public Shader
{
public:
	WaterHeightShader(std::string name);
	WaterHeightShader(std::string name, std::string fragName);
	~WaterHeightShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadWaterHeightMapDistWidth(float);
	void loadWaterHeightMapDistHeight(float);
	void loadSin45(float);
	void loadAttenuation(float);

private:
	GLuint m_location_waterHeightMapDistance_W; 
	GLuint m_location_waterHeightMapDistance_H;
	GLuint m_location_sin45;
	GLuint m_location_attenuation;
};

