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
	m_location_model = getUniformLocation("model");
	m_location_view = getUniformLocation("view");
	m_location_projection = getUniformLocation("projection");
	m_location_ODWHMR = getUniformLocation("ODWHMR");
}

void WaterHeightShader::loadModelMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_model, mat);
}

void WaterHeightShader::loadViewMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_view, mat);
}

void WaterHeightShader::loadProjectionMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_projection, mat);
}

void WaterHeightShader::loadODWHMR(float resolution)
{
	Shader::loadFloat(m_location_ODWHMR, resolution);
}