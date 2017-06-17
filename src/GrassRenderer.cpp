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
	static GLfloat vertices[DENSITY*4];

	std::mt19937 rng; //random number generator
	rng.seed(std::random_device()());
	std::uniform_real_distribution<float> dist(-0.6f, 0.6f);
	std::uniform_real_distribution<float> anglDist(-45.0f, 45.0f);

	for(int i = 0; i<DENSITY*4;i=i+4){ //create DENSITY grass coordinates in a 1.0 by 1.0 area at the origin
		vertices[i] = dist(rng);
		vertices[i+1] = 0;
		vertices[i+2] = dist(rng);
		vertices[i+3] = anglDist(rng);//store an angle in the 4th parameter
		
	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);


	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	glGenBuffers(1, &m_instanceVBO);

}

void GrassRenderer::render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition) {
	//std::cout << cameraPosition.x << " " << cameraPosition.z << "\n";
	//calculate offsets for instanced rendering
	//glm::vec3 translations[10000];

	//Nigel: should use raw data when dealing with OpenGL
	std::vector<GLfloat> translations(10000 * 3); //Nigel:  x3 make space for x, y, z components

	int index =0;
	for(int z = cameraPosition.z-50; z<cameraPosition.z+50;++z){ //create a 100x100 square around the cameraPosition
		for (int x = cameraPosition.x-50; x<cameraPosition.x+50;++x){
			if (index >= 10000 * 3) {//Nigel: This fixes segfualt hopefully. need to be more carefull in this loop.
				goto exit_label; // This is horrible fix later
			}
			glm::vec3 translation;		
			translation.x = x;
			translation.z = z;
			translation.y = world.heightAt(x,z);//and take the coordinates to be used as a translation vector for the grass instances
			//translation.y = 0.0f;
			//translations[index++]=translation;
			translations[index++] = translation.x;//Nigel:  since where store all x y z in single array
			translations[index++] = translation.y;
			translations[index++] = translation.z;
			//glm::vec3 normal =  world.normalAt(float worldX, float worldZ) Nigel: could be useful for lighting
		}
	}

	exit_label:;
	
	//TODO perform 2 pass rendering with  the first pass using culling shaders


	//render
	m_shader.use();

	m_shader.loadProjectionMatrix(projection);
	m_shader.loadModelMatrix(model);
	m_shader.loadViewMatrix(view);
	m_shader.loadCameraPosition(cameraPosition);

    	//create the instance offset VBO
    	glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 10000, &translations[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 10000 * 3, translations.data(), GL_DYNAMIC_DRAW); //Nigel: now using data contained in vector
													//sizeof(GLfloat) * 10000 * 3
    	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(m_VAO);

	glEnableVertexAttribArray(1);
    	glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO); // this attribute comes from a different vertex buffer
    	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.


	glDrawArraysInstanced(GL_POINTS, 0, DENSITY,10000);
	glBindVertexArray(0);


	m_shader.stop();
}


GrassRenderer::~GrassRenderer()
{
}
