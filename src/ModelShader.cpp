#include "ModelShader.hpp"



ModelShader::ModelShader(std::string name) : Shader(name)
{
	getAllUniformLocations();
}


ModelShader::~ModelShader()
{
}

void ModelShader::bindAttributes()
{
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoords");
	Shader::bindAttribute(2, "normal");
}

void ModelShader::getAllUniformLocations()
{
	mlocation_model = Shader::getUniformLocation("model");
	mlocation_view = Shader::getUniformLocation("view");
	mlocation_projection = Shader::getUniformLocation("projection");
}


void ModelShader::loadModelMatrix(glm::mat4 & mat)
{
	Shader::loadMatrix(mlocation_model, mat);
}

void ModelShader::loadViewMatrix(glm::mat4 & mat)
{
	Shader::loadMatrix(mlocation_view, mat);
}

void ModelShader::loadProjectionMatrix(glm::mat4 & mat)
{
	Shader::loadMatrix(mlocation_projection, mat);
}