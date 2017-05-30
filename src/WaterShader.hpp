#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class WaterShader : public Shader
{
public:
	WaterShader(std::string name);
	~WaterShader();

	void bindAttributes();

	void getAllUniformLocations();

	void loadModelMatrix(glm::mat4 mat);

	void loadViewMatrix(glm::mat4 mat);

	void loadProjectionMatrix(glm::mat4 mat);

	void loadCameraPosition(glm::vec3 & p);

private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
	GLuint m_location_cameraPosition;
};

