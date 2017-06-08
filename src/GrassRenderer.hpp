#pragma once

#include <GL/glew.h>
#include "Shader.hpp"

class GrassRenderer
{
public:
	GrassRenderer();
	~GrassRenderer();

	void render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition);
private:

	Shader m_shader;
	GLuint m_VBO;
	GLuint m_VAO;
};
 
