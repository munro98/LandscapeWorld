#include "GrassRenderer.hpp"
#include "GrassShader.hpp"
#include <cmath>
#include <random>

GrassRenderer::GrassRenderer() : m_shader(GrassShader("grassShader"))
{
	m_shader.use();
	//Update uniforms here
	m_shader.stop();

	//TODO generate an area of grass to instance over the scene
	static GLfloat vertices[DENSITY*3];

	std::mt19937 rng; //random number generator
	rng.seed(std::random_device()());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	for(int i = 0; i<DENSITY*3;i=i+3){
		vertices[i] = dist(rng);
		vertices[i+1] = 0;
		vertices[i+2] = dist(rng);
	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);


	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void GrassRenderer::render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition) {
	m_shader.use();

	m_shader.loadProjectionMatrix(projection);
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);
	m_shader.loadCameraPosition(cameraPosition);


	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, DENSITY);
	glBindVertexArray(0);


	m_shader.stop();
}


GrassRenderer::~GrassRenderer()
{
}
