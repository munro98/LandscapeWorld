#pragma once
#include "Shader.hpp"
class SkydomeShader :
	public Shader
{
public:
	SkydomeShader(std::string name);
	~SkydomeShader();
	void getAllUniformLocations();
	void loadModelMatrix(glm::mat4 & mat);
	void loadViewMatrix(glm::mat4 & mat);
	void loadProjectionMatrix(glm::mat4 & mat);
private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
};

