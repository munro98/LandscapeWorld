#include "GrassRenderer.hpp"
#include "GrassShader.hpp"
#include "World.hpp"
#include <cmath>
#include <random>

GrassRenderer::GrassRenderer(World &world) : m_shader(GrassShader("grassShader")),world(world) 
{
	m_shader.use();
	//Update uniforms here
	m_shader.stop();

	//generate an area of grass to instance over the scene
	static GLfloat vertices[DENSITY*3];

	std::mt19937 rng; //random number generator
	rng.seed(std::random_device()());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	for(int i = 0; i<DENSITY*3;i=i+3){ //create DENSITY grass coordinates in a 1 by 1 area at the origin
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

	//calculate offsets for instanced rendering
	glm::vec3 translations[10000];
	int index =0;
	for(int z = cameraPosition.z-50; z<cameraPosition.z+50;++z){ //create a 100x100 square around the cameraPosition
		for (int x = cameraPosition.x-50; x<cameraPosition.x+50;++x){
			glm::vec3 translation;		
			translation.x = x;
			translation.z = z;
			translation.y = world.heightAt(x,z);//and take the coordinates to be used as a translation vector for the grass instances
			translations[index++]=translation;
		}
	}
	
	//render
	m_shader.use();

	m_shader.loadProjectionMatrix(projection);
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);
	m_shader.loadCameraPosition(cameraPosition);

    	unsigned int instanceVBO;//create the instance offset VBO
    	glGenBuffers(1, &instanceVBO);
    	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 10000, &translations[0], GL_STATIC_DRAW);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(m_VAO);

	glEnableVertexAttribArray(1);
    	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.


	glDrawArraysInstanced(GL_POINTS, 0, DENSITY,10000);
	glBindVertexArray(0);


	m_shader.stop();
}


GrassRenderer::~GrassRenderer()
{
}
