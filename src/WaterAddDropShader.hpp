#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class WaterAddDropShader : public Shader
{
public:
	WaterAddDropShader(std::string name);
	~WaterAddDropShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadModelMatrix(glm::mat4 mat);
	void loadViewMatrix(glm::mat4 mat);
	void loadProjectionMatrix(glm::mat4 mat);

	void loadDropRadius(float);
	void loadDropPosition(glm::vec2 &p);

private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
	GLuint m_location_dropRadius;
	GLuint m_location_dropPosition;
};

