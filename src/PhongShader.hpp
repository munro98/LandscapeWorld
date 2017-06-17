#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class PhongShader : public Shader
{
public:
	PhongShader(std::string name);
	~PhongShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadModelMatrix(glm::mat4 mat);
	void loadViewMatrix(glm::mat4 mat);
	void loadProjectionMatrix(glm::mat4 mat);

	void loadCameraPosition(glm::vec3 &p);
	void loadLightDirection(glm::vec3 &p);
	void loadMaterialAmbient(glm::vec3 &p);
	void loadMaterialDiffuse(glm::vec3 &p);
	void loadMaterialSpecular(glm::vec3 &p);
	void loadMaterialShininess(float shinines);
	void loadLightAmbient(glm::vec3 &p);
	void loadLightDiffuse(glm::vec3 &p);
	void loadLightSpecular(glm::vec3 &p);
private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
	GLuint m_location_lightDirection;
	GLuint m_location_cameraPosition;
	GLuint m_location_materila_ambient;
	GLuint m_location_materila_diffuse;
	GLuint m_location_materila_specular;
	GLuint m_location_materila_shininess;
	GLuint m_location_light_ambient;
	GLuint m_location_light_diffuse;
	GLuint m_location_light_specular;
};

