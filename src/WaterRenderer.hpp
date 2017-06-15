#pragma once
#include "WaterShader.hpp"
#include "WaterHeightShader.hpp"
#include "WaterNormalShader.hpp"
#include "WaterAddDropShader.hpp"
#include "FrameBufObj.hpp"
#include "PhongShader.hpp"

class WaterRenderer
{
public:
	// Constructor
	WaterRenderer(glm::mat4& projection, glm::vec3& lightPosition);
	// Destructor
	~WaterRenderer();
	// renders the actual scene
	void render(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection, glm::vec3 & cameraPosition, float dropSize, float rainIntensity);

	// adds a drop on to the plane
	void addDrop(float x, float y, float dropRadius);

private:
	// The water mesh resolution
	const int WaterMeshResolution_Width = 128;
	const int WaterMeshResolution_Height = 128;

	// The WaterHeightMapt resolution
	const int WaterHeightMapResolution_Width = 128;
	const int WaterHeightMapResolution_Height = 128;

	// The WaterNormalMap resolution
	const int WaterNormalMapResolution_Width = 256;
	const int WaterNormalMapResolution_Height = 256;

	// The WaterHeightMap textures and buffers
	GLuint _waterHeightMapTextures[2];
	FrameBufObj _waterHeightMapFrameBuffers[2];

	// The WaterNormalMap texture and buffer
	GLuint _waterNormalMapTexture;
	FrameBufObj _waterNormalMapFrameBuffer;

	// The Shaders
	PhongShader _phongShader;
	WaterShader _waterShader;
	WaterHeightShader _waterHeightShader;
	WaterNormalShader _waterNormalShader;
	WaterAddDropShader _waterAddDropShader;

	Mesh* _bathtubMesh;

	// Variables to keep track of time for constant update 
	// of the height and normal map to maintain constant speed
	float _updateDiff = 0;
	float _rainDiff = 0;
	float _lastTime = 0;

	// WaterHeightMapp uses double buffering, this holds the index of the active buffer
	GLuint _waterHeightMapId = 0;

	// Buffer Id's
	GLuint _waterPlaneBuffId = 0;
	GLuint _squareBuffId = 0;

	// Amount of the triangels in the water plane
	int _triangleVerticesCount = 0;

	// Initializes the water pane
	void initWaterPlane();

	// Initializes the WaterHeightMaps
	void initWaterHeightMaps();

	// Initializes the WaterNormalMap
	void initWaterNormalMap();

	// Initalizes the phong shader for the bathtub
	void initPhongShader(glm::mat4& projection, glm::vec3& lightPosition);

	// Initializes the water shader
	void initWaterShader(glm::mat4& projection, glm::vec3& lightPosition);

	// Initializes the water height map shader
	void initWaterHeightMapShader();

	// Initializes the water normal map shader
	void initWaterNormalMapShader();

	// Updates the water height map
	void updateWaterHeightMap();

	// Updates the water normal map
	void updateWaterNormalMap();

	// Renders the bathtub
	void renderBathtub(glm::mat4 & view, glm::mat4 & model, glm::mat4 & projection, glm::vec3& cameraPosition);

	// Generates an TextureID and creates an empty thexture for later updates by the shaders
	static GLuint createEmptyTexture(GLuint w, GLuint h);

	// Sets um a simple square to be drwan over the whole screen. This
	// is mainly used to trigger the update of the height maps
	static void initSquareGeometry(GLuint* vao);

	// Actually draws the square on to the screen
	static void drawSquare(GLuint);
};

