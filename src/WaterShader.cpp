#include "WaterShader.hpp"



WaterShader::WaterShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


WaterShader::~WaterShader()
{
}

void WaterShader::bindAttributes()
{
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoords");
	Shader::bindAttribute(2, "normal");
}

void WaterShader::getAllUniformLocations()
{
	m_location_model = Shader::getUniformLocation("model");
	m_location_view = Shader::getUniformLocation("view");
	m_location_projection = Shader::getUniformLocation("projection");
	m_location_cameraPosition = Shader::getUniformLocation("cameraPosition");
	m_location_WaterHightMap = getUniformLocation("WaterHeightMap");
	m_location_WaterNormalMap = getUniformLocation("WaterNormalMap");
	m_location_LightPosition = getUniformLocation("LightPosition");
}

void WaterShader::loadModelMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_model, mat);
}

void WaterShader::loadViewMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_view, mat);
}

void WaterShader::loadProjectionMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_projection, mat);
}

void WaterShader::loadCameraPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_cameraPosition, p);
}

void WaterShader::loadLightPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_LightPosition, p);
}

void WaterShader::loadWaterHightMap(GLuint i)
{
	loadInt(m_location_WaterHightMap, i);
}

void WaterShader::loadWaterNormalMap(GLuint i)
{
	loadInt(m_location_WaterNormalMap, i);
}
