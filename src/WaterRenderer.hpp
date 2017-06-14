#pragma once
#include "WaterShader.hpp"
#include "WaterHeightShader.hpp"
#include "WaterNormalShader.hpp"
#include "WaterAddDropShader.hpp"
#include "FrameBufObj.hpp"

class WaterRenderer
{
public:
	WaterRenderer(glm::vec3& lightPosition);
	~WaterRenderer();

	void render(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection, glm::vec3 & cameraPosition);
	void addDrop();

private:
	const int WaterMeshResolution_Width = 128;
	const int WaterHeightMapResolution_Width = 128;
	const int WaterHeightMapResolution_Height = 128;
	const int WaterNormalMapResolution_Width = 256;
	const int WaterNormalMapResolution_Height = 256;

	WaterShader _waterShader;
	WaterHeightShader _waterHeightShader;
	WaterNormalShader _waterNormalShader;
	WaterAddDropShader _waterAddDropShader;

	float _diff = 0;
	float _lastTime = 0;

	//Mesh* m_mesh;
	//Texture* m_texture;
	GLuint _waterHeightMapTextures[2];
	FrameBufObj _waterHeightMapFrameBuffers[2];

	GLuint _waterNormalMapTexture;
	FrameBufObj _waterNormaMapFrameBuffer;

	GLuint _waterPlaneBuffId;
	GLuint _waterHeightMapId = 0;
	GLuint _squareBuffId = 0;

	int _triangleVerticesCount = 0;

	void initWaterPlane();
	void initWaterHeightMaps();
	void initWaterNormalMap();
	static GLuint createEmptyTexture(GLuint w, GLuint h);
	static void initSquareGeometry(GLuint* vao);
	static void drawSquare(GLuint);
};

