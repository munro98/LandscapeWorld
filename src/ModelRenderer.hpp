#pragma once
#include "Shader.hpp"
#include "ModelShader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

class ModelRenderer
{
public:
	ModelRenderer(glm::mat4 projectionMatrix);
	~ModelRenderer();

	void render(glm::mat4 view, glm::mat4 model, glm::mat4 projection);

private:
	ModelShader m_shader;
	Mesh* m_mesh;
	Texture* m_texture;
};

