 #include "GrassRenderer.hpp"
#include <cmath>
#include <random>

GrassRenderer::GrassRenderer() : m_shader(Shader("grassShader"))
{
	m_shader.use();
	//Update uniforms here
	m_shader.stop();

	//TODO generate an area of grass to instance over the scene

	const int density = 50; //number of grass instances over square unit of worldspace
	
	static GLfloat vertices[density*2];
	
	std::mt19937 rng; //random number generator
	rng.seed(std::random_device()());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	
	for(int i = 0; i<density*2;++i){
		vertices[i] = dist(rng);
	}
	
	// Send simple triangle to GPU
	//static GLfloat vertices[] = {
	//	-0.5f,-0.5f,0.0f,
	//	0.5f,-0.5f,0.0f,
	//	0.0f,0.5f,0.0f
	//};


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
	//Draw a few points
	m_shader.use();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, 3);
	glBindVertexArray(0);
	m_shader.stop();
}


GrassRenderer::~GrassRenderer()
{
}
