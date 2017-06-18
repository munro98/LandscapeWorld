#include "WaterHeightShader.hpp"



WaterHeightShader::WaterHeightShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


WaterHeightShader::~WaterHeightShader()
{
}

void WaterHeightShader::bindAttributes()
{
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
}

void WaterHeightShader::getAllUniformLocations()
{
	m_location_waterHeightMapDistance_W = getUniformLocation("waterHeightMapDistance_W");
	m_location_waterHeightMapDistance_H = getUniformLocation("waterHeightMapDistance_H");
}

void WaterHeightShader::loadwaterHeightMapDistWidth(float resolution)
{
	Shader::loadFloat(m_location_waterHeightMapDistance_W, resolution);
}

void WaterHeightShader::loadwaterHeightMapDistHeight(float resolution)
{
	Shader::loadFloat(m_location_waterHeightMapDistance_H, resolution);
}