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
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoords");
	Shader::bindAttribute(2, "normal");
}

void WaterNormalShader::getAllUniformLocations()
{
	m_location_model = Shader::getUniformLocation("model");
	m_location_view = Shader::getUniformLocation("view");
	m_location_projection = Shader::getUniformLocation("projection");
	m_location_cameraPosition = Shader::getUniformLocation("cameraPosition");
	m_location_WaterHightMap = getUniformLocation("WaterHeightMap");
	m_location_WaterNormalMap = getUniformLocation("WaterNormalMap");
	m_location_LightPosition = getUniformLocation("LightPosition");
}

void WaterNormalShader::loadModelMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_model, mat);
}

void WaterNormalShader::loadViewMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_view, mat);
}

void WaterNormalShader::loadProjectionMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_projection, mat);
}

void WaterNormalShader::loadCameraPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_cameraPosition, p);
}

void WaterNormalShader::loadLightPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_LightPosition, p);
}

void WaterNormalShader::loadWaterHightMap(GLuint i)
{
	loadInt(m_location_WaterHightMap, i);
}

void WaterNormalShader::loadWaterNormalMap(GLuint i)
{
	loadInt(m_location_WaterNormalMap, i);
}
