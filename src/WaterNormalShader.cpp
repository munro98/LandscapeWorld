#include "WaterNormalShader.hpp"



WaterNormalShader::WaterNormalShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


WaterNormalShader::~WaterNormalShader()
{
}

void WaterNormalShader::bindAttributes()
{
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
}

void WaterNormalShader::getAllUniformLocations()
{
	m_location_waterNormalMapDistance_W = getUniformLocation("waterNormalMapDistance_W");
	m_location_waterNormalMapDistance_H = getUniformLocation("waterNormalMapDistance_H");
	m_location_defaultHeight = getUniformLocation("defaultHeight");
}

void WaterNormalShader::loadwaterNormMapDistWidth(float resolution)
{
	loadFloat(m_location_waterNormalMapDistance_W, resolution);
}

void WaterNormalShader::loadwaterNormMapDistHeight(float resolution)
{
	loadFloat(m_location_waterNormalMapDistance_H, resolution);
}

void WaterNormalShader::loadDefaultHeight(float p)
{
	loadFloat(m_location_defaultHeight, p);
}