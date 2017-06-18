#include "WaterShader.hpp"

using namespace glm;
using namespace std;

WaterShader::WaterShader(string name) : WaterShader(name, name)
{
	getAllUniformLocations();
}

WaterShader::WaterShader(string name, string fragmentName) : Shader(name, fragmentName)
{
	getAllUniformLocations();
}

WaterShader::~WaterShader()
{
}

void WaterShader::bindAttributes()
{
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
}

void WaterShader::getAllUniformLocations()
{
	m_location_model = getUniformLocation("model");
	m_location_view = getUniformLocation("view");
	m_location_projection = getUniformLocation("projection");
	m_location_cameraPosition = getUniformLocation("cameraPosition");
	m_location_WaterHeightMap = getUniformLocation("WaterHeightMap");
	m_location_WaterNormalMap = getUniformLocation("WaterNormalMap");
	m_location_LightPosition = getUniformLocation("LightPosition");
	m_location_WaterColour = getUniformLocation("waterColour");
	m_location_ShowColour = getUniformLocation("useColour");
}

void WaterShader::loadModelMatrix(mat4 & mat)
{
	loadMatrix(m_location_model, mat);
}

void WaterShader::loadViewMatrix(mat4 & mat)
{
	loadMatrix(m_location_view, mat);
}

void WaterShader::loadProjectionMatrix(mat4 & mat)
{
	loadMatrix(m_location_projection, mat);
}

void WaterShader::loadCameraPosition(vec3& p)
{
	loadVector(m_location_cameraPosition, p);
}

void WaterShader::loadLightPosition(vec3& p)
{
	loadVector(m_location_LightPosition, p);
}

void WaterShader::loadWaterHeightMap(GLuint i)
{
	loadInt(m_location_WaterHeightMap, i);
}

void WaterShader::loadWaterNormalMap(GLuint i)
{
	loadInt(m_location_WaterNormalMap, i);
}

void WaterShader::loadWaterColour(vec4 &p)
{
	loadVector(m_location_WaterColour, p);
}

void WaterShader::loadShowColour(bool showColor)
{
	//loadBool(m_location_WaterColour, showColor);
	loadInt(m_location_WaterColour, showColor ? 1: 0);
}
