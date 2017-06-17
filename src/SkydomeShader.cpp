#include "SkydomeShader.hpp"



SkydomeShader::SkydomeShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


SkydomeShader::~SkydomeShader()
{
}


void SkydomeShader::getAllUniformLocations()
{
	m_location_model = Shader::getUniformLocation("model");
	m_location_view = Shader::getUniformLocation("view");
	m_location_projection = Shader::getUniformLocation("projection");
}

void SkydomeShader::loadModelMatrix(glm::mat4 & mat)
{
	Shader::loadMatrix(m_location_model, mat);
}

void SkydomeShader::loadViewMatrix(glm::mat4 & mat)
{
	Shader::loadMatrix(m_location_view, mat);
}

void SkydomeShader::loadProjectionMatrix(glm::mat4 & mat)
{
	Shader::loadMatrix(m_location_projection, mat);
}