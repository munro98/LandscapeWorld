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
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
}

void WaterNormalShader::getAllUniformLocations()
{
	m_location_model = getUniformLocation("model");
	m_location_view = getUniformLocation("view");
	m_location_projection = getUniformLocation("projection");
	m_location_waterNormalMapResolution_W = getUniformLocation("waterNormalMapResolution_W");
	m_location_waterNormalMapResolution_H = getUniformLocation("waterNormalMapResolution_H");
	m_location_WMSDWNMRM2 = getUniformLocation("WMSDWNMRM2");
}

void WaterNormalShader::loadModelMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_model, mat);
}

void WaterNormalShader::loadViewMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_view, mat);
}

void WaterNormalShader::loadProjectionMatrix(glm::mat4 mat)
{
	loadMatrix(m_location_projection, mat);
}

void WaterNormalShader::loadwaterNormalMapResolutionWidth(float resolution)
{
	loadFloat(m_location_waterNormalMapResolution_W, resolution);
}

void WaterNormalShader::loadwaterNormalMapResolutionHeight(float resolution)
{
	loadFloat(m_location_waterNormalMapResolution_H, resolution);
}

void WaterNormalShader::loadWMSDWNMRM2(float p)
{
	loadFloat(m_location_WMSDWNMRM2, p);
}