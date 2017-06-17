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
	m_location_sin45 = getUniformLocation("sin45");
	m_location_attenuation = getUniformLocation("attenuation");
}

void WaterHeightShader::loadWaterHeightMapDistWidth(float resolution)
{
	loadFloat(m_location_waterHeightMapDistance_W, resolution);
}

void WaterHeightShader::loadWaterHeightMapDistHeight(float resolution)
{
	loadFloat(m_location_waterHeightMapDistance_H, resolution);
}

void WaterHeightShader::loadSin45(float value)
{
	loadFloat(m_location_sin45, value);
}

void WaterHeightShader::loadAttenuation(float attenuation)
{
	loadFloat(m_location_attenuation, attenuation);
}