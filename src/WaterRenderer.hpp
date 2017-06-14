#pragma once
#include "WaterShader.hpp"
#include "Texture.hpp"
#include "WaterHeightShader.hpp"
#include "WaterNormalShader.hpp"
#include "WaterAddDropShader.hpp"
#include "FrameBuffer.hpp"

#define WMR 128 // water mesh resolution
#define WHMR_W 128 // water height map resolution
#define WHMR_H 128 // water height map resolution
#define WNMR_W 256 // water normal map resolution
#define WNMR_H 256 // water normal map resolution

class WaterRenderer
{
public:
	//WaterRenderer(glm::mat4& projectionMatrix);
	WaterRenderer(glm::vec3& lightPosition);
	~WaterRenderer();

	void render(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection, glm::vec3 & cameraPosition);
	void addDrop();

private:
	WaterShader m_waterShader;
	WaterHeightShader m_waterHightShader;
	WaterNormalShader m_waterNormalShader;
	WaterAddDropShader m_waterAddDropShader;

	float _diff = 0;
	float _lastTime = 0;

	//Mesh* m_mesh;
	//Texture* m_texture;
	GLuint WaterHeightMapTextures[2];
	FrameBuffer WaterHeightMapFrameBuffers[2];

	GLuint WaterNormalMapTexture;
	FrameBuffer WaterNormaMapFrameBuffer;

	GLuint WaterVBO;
	GLuint WHMID = 0;
	GLuint FBO;
	GLuint mQuadVAO = 0;

	int QuadsVerticesCount = 0;

	void initTriangles();
	void initWaterHeightMaps();
	void initWaterNormalMap();
	GLuint createEmptyTexture2D(GLuint w, GLuint h);
	void initQuadGeometry(GLuint* vao);
	void drawQuad(GLuint);
};

