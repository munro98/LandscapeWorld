#include "WaterAddDropShader.hpp"

WaterAddDropShader::WaterAddDropShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}

WaterAddDropShader::~WaterAddDropShader()
{
}

void WaterAddDropShader::bindAttributes()
{
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
}

void WaterAddDropShader::getAllUniformLocations()
{
	m_location_dropRadius = getUniformLocation("dropRadius");
	m_location_dropPosition = getUniformLocation("dropPosition");
}

void WaterAddDropShader::loadDropRadius(float radius)
{
	loadFloat(m_location_dropRadius, radius);
}

void WaterAddDropShader::loadDropPosition(glm::vec2& p)
{
	loadVector(m_location_dropPosition, p);
}