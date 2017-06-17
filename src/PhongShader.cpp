#include "PhongShader.hpp"

using namespace glm;
using namespace std;

PhongShader::PhongShader(string name) : Shader(name)
{
	getAllUniformLocations();
}


PhongShader::~PhongShader()
{
}

void PhongShader::bindAttributes()
{
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
}

void PhongShader::getAllUniformLocations()
{
	m_location_model = getUniformLocation("model");
	m_location_view = getUniformLocation("view");
	m_location_projection = getUniformLocation("projection");
	m_location_cameraPosition = getUniformLocation("cameraPosition");
	m_location_lightDirection = getUniformLocation("lightPosition");
	m_location_materila_ambient = getUniformLocation("material.ambient");
	m_location_materila_diffuse = getUniformLocation("material.diffuse");
	m_location_materila_specular = getUniformLocation("material.specular");
	m_location_materila_shininess = getUniformLocation("material.shininess");
	m_location_light_ambient = getUniformLocation("light.ambient");
	m_location_light_diffuse = getUniformLocation("light.diffuse");
	m_location_light_specular = getUniformLocation("light.specular");
}

void PhongShader::loadModelMatrix(mat4 & mat)
{
	loadMatrix(m_location_model, mat);
}

void PhongShader::loadViewMatrix(mat4 & mat)
{
	loadMatrix(m_location_view, mat);
}

void PhongShader::loadProjectionMatrix(mat4 & mat)
{
	loadMatrix(m_location_projection, mat);
}

void PhongShader::loadLightDirection(vec3& p)
{
	loadVector(m_location_lightDirection, p);
}

void PhongShader::loadMaterialAmbient(vec3 &p)
{
	loadVector(m_location_materila_ambient, p);
}

void PhongShader::loadMaterialDiffuse(vec3 &p)
{
	loadVector(m_location_materila_diffuse, p);
}

void PhongShader::loadMaterialSpecular(vec3 &p)
{
	loadVector(m_location_materila_specular, p);
}

void PhongShader::loadMaterialShininess(float shinines)
{
	loadFloat(m_location_materila_shininess, shinines);
}

void PhongShader::loadCameraPosition(vec3 &p)
{
	loadVector(m_location_cameraPosition, p);
}

void PhongShader::loadLightAmbient(vec3 &p)
{
	loadVector(m_location_light_ambient, p);
}

void PhongShader::loadLightDiffuse(vec3 &p)
{
	loadVector(m_location_light_diffuse, p);
}

void PhongShader::loadLightSpecular(vec3 &p)
{
	loadVector(m_location_light_specular, p);
}