#pragma once
#include <glm/glm.hpp>

#include "Loader.hpp"
#include "SkydomeShader.hpp"


class SkydomeRenderer
{
public:
	SkydomeRenderer(glm::mat4 projectionMatrix);
	~SkydomeRenderer();
	void render(glm::mat4 view, glm::mat4 & model);
private:
	static std::vector<float> m_vertices;
	Mesh *m_cube;
	SkydomeShader m_shader;

	
};

