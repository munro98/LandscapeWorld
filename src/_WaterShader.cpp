#include "_WaterShader.hpp"



WaterShader_Old::WaterShader_Old(std::string name) : Shader(name)
{
	getAllUniformLocations();
}

WaterShader_Old::~WaterShader_Old()
{
}

void WaterShader_Old::bindAttributes()
{
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoords");
	Shader::bindAttribute(2, "normal");
}

void WaterShader_Old::getAllUniformLocations()
{
	m_location_model = Shader::getUniformLocation("model");
	m_location_view = Shader::getUniformLocation("view");
	m_location_projection = Shader::getUniformLocation("projection");

	m_location_cameraPosition = Shader::getUniformLocation("cameraPosition");
}

void WaterShader_Old::loadModelMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_model, mat);
}

void WaterShader_Old::loadViewMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_view, mat);
}

void WaterShader_Old::loadProjectionMatrix(glm::mat4 mat)
{
	Shader::loadMatrix(m_location_projection, mat);
}

void WaterShader_Old::loadCameraPosition(glm::vec3& p)
{
	Shader::loadVector(m_location_cameraPosition, p);
}