#include "GrassShader.hpp"



GrassShader::GrassShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


GrassShader::~GrassShader()
{
}

void GrassShader::bindAttributes()
{
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoords");
	Shader::bindAttribute(2, "normal");
}

void GrassShader::getAllUniformLocations()
{
	m_location_model = Shader::getUniformLocation("model");
	m_location_view = Shader::getUniformLocation("view");
	m_location_projection = Shader::getUniformLocation("projection");
	m_location_cameraPosition = Shader::getUniformLocation("cameraPosition");

	m_time = Shader::getUniformLocation("time");
}

void GrassShader::loadModelMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_model, mat);
}

void GrassShader::loadViewMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_view, mat);
}

void GrassShader::loadProjectionMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_projection, mat);
}

void GrassShader::loadCameraPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_cameraPosition, p);
} 

void GrassShader::loadTime(float t){
	Shader::loadFloat(m_time, t);
}
