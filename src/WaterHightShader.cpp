#include "WaterHightShader.hpp"



WaterHightShader::WaterHightShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


WaterHightShader::~WaterHightShader()
{
}

void WaterHightShader::bindAttributes()
{
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoords");
	Shader::bindAttribute(2, "normal");
}

void WaterHightShader::getAllUniformLocations()
{
	m_location_model = Shader::getUniformLocation("model");
	m_location_view = Shader::getUniformLocation("view");
	m_location_projection = Shader::getUniformLocation("projection");
	m_location_cameraPosition = Shader::getUniformLocation("cameraPosition");
	m_location_WaterHightMap = getUniformLocation("WaterHeightMap");
	m_location_WaterNormalMap = getUniformLocation("WaterNormalMap");
	m_location_LightPosition = getUniformLocation("LightPosition");
}

void WaterHightShader::loadModelMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_model, mat);
}

void WaterHightShader::loadViewMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_view, mat);
}

void WaterHightShader::loadProjectionMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_projection, mat);
}

void WaterHightShader::loadCameraPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_cameraPosition, p);
}

void WaterHightShader::loadLightPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_LightPosition, p);
}

void WaterHightShader::loadWaterHightMap(GLuint i)
{
	loadInt(m_location_WaterHightMap, i);
}

void WaterHightShader::loadWaterNormalMap(GLuint i)
{
	loadInt(m_location_WaterNormalMap, i);
}
