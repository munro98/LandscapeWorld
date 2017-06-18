#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class ModelShader : public Shader
{
public:
	ModelShader(std::string name);
	~ModelShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadModelMatrix(glm::mat4 & mat);
	void loadViewMatrix(glm::mat4 & mat);
	void loadProjectionMatrix(glm::mat4 & mat);

private:
	GLuint mlocation_model;
	GLuint mlocation_view;
	GLuint mlocation_projection;
};

