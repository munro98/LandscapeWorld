#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class WaterHeightShader : public Shader
{
public:
	WaterHeightShader(std::string name);
	~WaterHeightShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadModelMatrix(glm::mat4 mat);
	void loadViewMatrix(glm::mat4 mat);
	void loadProjectionMatrix(glm::mat4 mat);

	void loadODWHMR(float);

private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
	GLuint m_location_ODWHMR;
};

