#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class GrassShader : public Shader
{
public:
	GrassShader(std::string name);
	~GrassShader();

	void bindAttributes();

	void getAllUniformLocations();

	void loadModelMatrix(glm::mat4 mat);

	void loadViewMatrix(glm::mat4 mat);

	void loadProjectionMatrix(glm::mat4 mat);

	void loadCameraPosition(glm::vec3 & p);

	void loadTime(float t);

private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
	GLuint m_location_cameraPosition;
	GLuint m_time;
}; 
