#pragma once
#include "Shader.hpp"
#include "WaterShader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

class WaterRenderer
{
public:
	WaterRenderer(glm::mat4& projectionMatrix);
	~WaterRenderer();

	void render(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection, glm::vec3 & cameraPosition);

	//void render(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection);

private:
	WaterShader m_shader;
	Mesh* m_mesh;
	Texture* m_texture;

	GLuint WaterHeightMaps[2];
};

