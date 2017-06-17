#pragma once

#include <GL/glew.h>
#include "Shader.hpp"
#include "GrassShader.hpp"
#include "World.hpp"


#define DENSITY 100 //grass blades per sqaure unit
class GrassRenderer
{
public:
	GrassRenderer(World &world);
	~GrassRenderer();

	void render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition);
private:
	World &world;
	GrassShader m_shader;
	GLuint m_VBO;
	GLuint m_VAO;

	GLuint m_instanceVBO;
};
 
