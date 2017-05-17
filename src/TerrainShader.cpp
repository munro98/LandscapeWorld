#include "TerrainShader.hpp"



TerrainShader::TerrainShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


TerrainShader::~TerrainShader()
{
}

void TerrainShader::bindAttributes()
{
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoords");
	Shader::bindAttribute(2, "normal");
}

void TerrainShader::getAllUniformLocations()
{
	m_location_model = Shader::getUniformLocation("model");
	m_location_view = Shader::getUniformLocation("view");
	m_location_projection = Shader::getUniformLocation("projection");

	m_location_lightPosition = Shader::getUniformLocation("lightPosition");
	m_location_lightColour = Shader::getUniformLocation("lightColour");


	m_location_camPos = Shader::getUniformLocation("camPos");
	//assert(m_location_lightPosition != 0);
	//assert(m_location_lightColour != 0);
}


void TerrainShader::loadModelMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_model, mat);
}

void TerrainShader::loadViewMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_view, mat);
}

void TerrainShader::loadProjectionMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_projection, mat);
}

void TerrainShader::loadLightPosition(glm::vec3 lightColour)
{
	Shader::loadVector(m_location_lightPosition, lightColour);
}

void TerrainShader::loadLightColour(glm::vec3 lightColour)
{
	Shader::loadVector(m_location_lightColour, lightColour);
}

void TerrainShader::loadCamPos(glm::vec3 camPos)
{
	Shader::loadVector(m_location_camPos, camPos);
}
