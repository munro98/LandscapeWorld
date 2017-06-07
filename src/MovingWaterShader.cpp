#include "MovingWaterShader.hpp"



MovingWaterShader::MovingWaterShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


MovingWaterShader::~MovingWaterShader()
{
}

void MovingWaterShader::bindAttributes()
{
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoords");
	Shader::bindAttribute(2, "normal");
}

void MovingWaterShader::getAllUniformLocations()
{
	m_location_model = Shader::getUniformLocation("model");
	m_location_view = Shader::getUniformLocation("view");
	m_location_projection = Shader::getUniformLocation("projection");
	m_location_cameraPosition = Shader::getUniformLocation("cameraPosition");
	m_location_WaterHightMap = getUniformLocation("WaterHeightMap");
	m_location_WaterNormalMap = getUniformLocation("WaterNormalMap");
	m_location_LightPosition = getUniformLocation("LightPosition");
}

void MovingWaterShader::loadModelMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_model, mat);
}

void MovingWaterShader::loadViewMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_view, mat);
}

void MovingWaterShader::loadProjectionMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_projection, mat);
}

void MovingWaterShader::loadCameraPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_cameraPosition, p);
}

void MovingWaterShader::loadLightPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_LightPosition, p);
}

void MovingWaterShader::loadWaterHightMap(GLuint i)
{
	loadInt(m_location_WaterHightMap, i);
}

void MovingWaterShader::loadWaterNormalMap(GLuint i)
{
	loadInt(m_location_WaterNormalMap, i);
}
