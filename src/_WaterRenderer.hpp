#pragma once
#include "Shader.hpp"
#include "_WaterShader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

class WaterRenderer_Old
{
public:
	WaterRenderer_Old(glm::mat4& projectionMatrix);
	~WaterRenderer_Old();

	void render(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection, glm::vec3 & cameraPosition);

	//void render(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection);

private:
	WaterShader_Old m_shader;
	Mesh* m_mesh;
	Texture* m_texture;

	GLuint WaterHeightMaps[2];
};

