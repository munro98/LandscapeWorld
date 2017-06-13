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
	m_location_model = getUniformLocation("model");
	m_location_view = getUniformLocation("view");
	m_location_projection = getUniformLocation("projection");
	m_location_dropRadius = getUniformLocation("DropRadius");
	m_location_position = getUniformLocation("Position");
}

void WaterAddDropShader::loadModelMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_model, mat);
}

void WaterAddDropShader::loadViewMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_view, mat);
}

void WaterAddDropShader::loadProjectionMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_projection, mat);
}

void WaterAddDropShader::loadDropRadius(float radius)
{
	loadFloat(m_location_dropRadius, radius);
}

void WaterAddDropShader::loadPosition(glm::vec2& p)
{
	loadVector(m_location_position, p);
}