#pragma once
#include "MovingWaterShader.hpp"
#include "Texture.hpp"

#define WMR 128 // water mesh resolution
#define WHMR_W 128 // water height map resolution
#define WHMR_H 128 // water height map resolution
#define WNMR_W 256 // water normal map resolution
#define WNMR_H 256 // water normal map resolution

class MovingWaterRenderer
{
public:
	//MovingWaterRenderer(glm::mat4& projectionMatrix);
	MovingWaterRenderer(glm::vec3& lightPosition);
	~MovingWaterRenderer();

	void render(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection, glm::vec3 & cameraPosition);

private:
	//Shader m_shader;
	MovingWaterShader m_shader;
	//Mesh* m_mesh;
	//Texture* m_texture;
	GLuint WaterHeightMaps[2];
	GLuint WaterNormalMap;
	GLuint WaterVBO;

	int QuadsVerticesCount = 0;

	void initTriangles();
	void initWaterHeightMaps(GLuint);
	void initWaterNormalMap(GLuint);
};

