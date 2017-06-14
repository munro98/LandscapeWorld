#include "WaterRenderer.hpp"
#include "WaterShader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"
#include <glm/gtc/matrix_transform.inl>
#include "GLFW/glfw3.h"

using namespace glm;

WaterRenderer::WaterRenderer(vec3& lightPosition) :
	m_waterShader(WaterShader("waterShader")),
	m_waterHightShader(WaterHeightShader("waterHeightMap")),
	m_waterNormalShader(WaterNormalShader("waterNormalMap")),
	m_waterAddDropShader(WaterAddDropShader("waterAddDrop"))
{
	// Initialize the three different maps
	//  - the plane
	initWaterPlane();
	//  - the hight map
	initWaterHeightMaps();
	//  - the normal map
	initWaterNormalMap();

	// Initializes the shaders
	//  - the water shader, wich is responsible to draw the water and its colours
	m_waterShader.use();
	m_waterShader.loadWaterHightMap(0);
	m_waterShader.loadWaterNormalMap(1);
	m_waterShader.loadLightPosition(lightPosition);
	vec4 waterColor = vec4(112.0 / 255.0, 143.0 / 255.0, 184.0 / 255.0, 1.0);
	m_waterShader.loadWaterColor(waterColor);
	m_waterShader.stop();

	//  - the water hight shader, wich will update the hight map
	m_waterHightShader.use();
	m_waterHightShader.loadODWHMR(1.0f / WHMR_W);
	m_waterHightShader.stop();

	//  - the water normal shader, wich will update the normal map
	m_waterNormalShader.use();
	m_waterNormalShader.loadODWNMR(1.0f / WNMR_W);
	m_waterNormalShader.loadWMSDWNMRM2(2.0f / WNMR_W * 2);
	m_waterNormalShader.stop();

	initSquareGeometry(&mQuadVAO);
}

WaterRenderer::~WaterRenderer()
{
	//for(int i = 0; i < 2; i++)
	//{
	//	WaterHeightMapFrameBuffers[i].destroy();
	//}

	//WaterNormaMapFrameBuffer.destroy();
}

void WaterRenderer::render(mat4& view, mat4& model, mat4& projection, vec3& cameraPosition)
{
	mat4 waterModel = mat4();
	//waterModel = rotate(waterModel, -45.0f, vec3(1, 0, 0));
	waterModel = translate(waterModel, vec3(50, 11, 50));
	waterModel = scale(waterModel, vec3(2, 2, 2));

	double currentTime = glfwGetTime();
	_diff += _lastTime == 0 ? 0 : currentTime - _lastTime;
	_lastTime = currentTime;

	if (_diff > 0.016)
	{
		_diff = 0;

		// Save current viewport settings
		int origViewport[4];
		glGetIntegerv(GL_VIEWPORT, origViewport);

		// Set viewport to exactly the hight and width of the WaterHeightMap resolution
		glViewport(0, 0, WHMR_W, WHMR_H);
		
		// Get the next id for the water hight buffer
		GLuint nextId = (WHMID + 1) % 2;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		// Bind the water hight map, use the HightMapShader and draw
		// a square to update the water hight map and unbind everything
		WaterHeightMapFrameBuffers[nextId].bind();
		m_waterHightShader.use();
		WaterHeightMapFrameBuffers[WHMID].bindColourTarget(0);

		drawSquare(mQuadVAO);
		m_waterHightShader.stop();

		FrameBufObj::resetBinding();

		// Set the next buffer
		WHMID = nextId;

		// Set viewport to exactly the hight and width of the WaterNormalMap resolution
		glViewport(0, 0, WNMR_W, WNMR_H);

		// Bind the WaterNormalMap to write into, use the WaterNormalShader and bind
		// the current heightMap to calculate the normals and then unbind everything
		WaterNormaMapFrameBuffer.bind();
		m_waterNormalShader.use();
		WaterHeightMapFrameBuffers[WHMID].bindColourTarget(0);
		drawSquare(mQuadVAO);
		m_waterNormalShader.stop();

		FrameBufObj::resetBinding();

		// reset the Viewport to the previouse values
		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}

	// Actual rendering
	// use the water shader and pass in the appropriate matrices
	m_waterShader.use();
	m_waterShader.loadProjectionMatrix(projection);
	m_waterShader.loadModelMatrix(waterModel);
	m_waterShader.loadViewMatrix(view);
	m_waterShader.loadCameraPosition(cameraPosition);

	// Bind the WaterHeightMap and the WaterNormalMap for the shaders use
	glActiveTexture(GL_TEXTURE0);
	WaterHeightMapFrameBuffers[WHMID].bindColourTarget(0);
	glActiveTexture(GL_TEXTURE1);
	WaterNormaMapFrameBuffer.bindColourTarget(0);

	// Draw the actual plane
	glBindVertexArray(WaterVBO);
	glDrawArrays(GL_TRIANGLES, 0, _triangleVerticesCount);
	glBindVertexArray(0);
	m_waterShader.stop();

	// Unbind everything
	glActiveTexture(GL_TEXTURE1);
	FrameBufObj::resetBinding();
	glActiveTexture(GL_TEXTURE0);
	FrameBufObj::resetBinding();
}

void WaterRenderer::addDrop()
{
	//if (x >= -1.0f && x <= 1.0f && y >= -1.0f && y <= 1.0f)
	{
		// Save the current viewport
		int origViewport[4];
		glGetIntegerv(GL_VIEWPORT, origViewport);

		// Set Viewport to with and height of the Water Map
		glViewport(0, 0, WMR, WMR);

		// Calculate the next ID for the WaterHightMap
		GLuint nextId = (WHMID + 1) % 2;
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		// Bind the next WaterHeightMap to write into
		WaterHeightMapFrameBuffers[nextId].bind();
		// Use the WaterDropShader and pass in the apropriate values
		m_waterAddDropShader.use();
		m_waterAddDropShader.loadDropRadius(0.05);
		vec2 dropPosition = vec2(0.5, 0.5);
		m_waterAddDropShader.loadPosition(dropPosition);
		// Load the current WaterHeightMap to read from
		WaterHeightMapFrameBuffers[WHMID].bindColourTarget(0);

		// Draw a square on the screen to update the WaterHeightMap
		drawSquare(mQuadVAO);

		// Unbind Shader and textures
		m_waterAddDropShader.stop();
		FrameBufObj::resetBinding();

		// Change WaterHeightMap buffers
		WHMID = nextId;

		// Reset Viewprot to the original values
		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}
}

void WaterRenderer::initWaterPlane()
{
	int WMRP1 = WMR + 1;
	vec3 *Vertices = new vec3[WMRP1 * WMRP1];
	
	float WMSDWMR = 2.0f / static_cast<float>(WMR);
	
	// Create triangel points
	for (int y = 0; y <= WMR; y++)
	{
		for (int x = 0; x <= WMR; x++)
		{
			Vertices[WMRP1 * y + x].x = x * WMSDWMR - 1.0f;
			Vertices[WMRP1 * y + x].y = 0.0f;
			Vertices[WMRP1 * y + x].z = 1.0f - y * WMSDWMR;
		}
	}
	
	std::vector<GLfloat> triangles;

	// draw the actual triangles
	for (int y = 0; y < WMR; y++)
	{
		int yp1 = y + 1;
	
		for (int x = 0; x < WMR; x++)
		{
			int xp1 = x + 1;
	
			int a = WMRP1 * y + x;
			int b = WMRP1 * y + xp1;
			int c = WMRP1 * yp1 + xp1;
			int d = WMRP1 * yp1 + x;
	
			triangles.push_back(Vertices[a].x);
			triangles.push_back(Vertices[a].y);
			triangles.push_back(Vertices[a].z);
			triangles.push_back(Vertices[b].x);
			triangles.push_back(Vertices[b].y);
			triangles.push_back(Vertices[b].z);
			triangles.push_back(Vertices[c].x);
			triangles.push_back(Vertices[c].y);
			triangles.push_back(Vertices[c].z);
	
			triangles.push_back(Vertices[d].x);
			triangles.push_back(Vertices[d].y);
			triangles.push_back(Vertices[d].z);
			triangles.push_back(Vertices[a].x);
			triangles.push_back(Vertices[a].y);
			triangles.push_back(Vertices[a].z);
			triangles.push_back(Vertices[c].x);
			triangles.push_back(Vertices[c].y);
			triangles.push_back(Vertices[c].z);
		}
	}

	// bind the triangles to the buffers
	glGenVertexArrays(1, &WaterVBO);
	glBindVertexArray(WaterVBO);

	GLuint t;
	glGenBuffers(1, &t);
	glBindBuffer(GL_ARRAY_BUFFER, t);
	glBufferData(GL_ARRAY_BUFFER, triangles.size() * 4, triangles.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	_triangleVerticesCount = triangles.size() / 3;
	
	// cleanup
	triangles.clear();
	delete[] Vertices;
}

void WaterRenderer::initWaterHeightMaps()
{
	glActiveTexture(GL_TEXTURE0);

	// save current settings:
	int view[4];
	glGetIntegerv(GL_VIEWPORT, view);

	// New viewport
	glViewport(0, 0, WHMR_W, WHMR_H);

	for (int i = 0; i < 2; i++)
	{
		// Setup the WaterHeightMapTextures and buffers
		WaterHeightMapTextures[i] = createEmptyTexture(WHMR_W, WHMR_H);

		WaterHeightMapFrameBuffers[i].createAndBind();
		WaterHeightMapFrameBuffers[i].attachTexture(0, WaterHeightMapTextures[i], WHMR_W, WHMR_H);
		WaterHeightMapFrameBuffers[i].setDrwaBuff();
		WaterHeightMapFrameBuffers[i].check();
	}

	// Unbind all the textures and buffers
	FrameBufObj::resetBinding();
	
	// Reset Viewport to previouse settings
	glViewport(view[0], view[1], view[2], view[3]);
}

void WaterRenderer::initWaterNormalMap()
{
	glActiveTexture(GL_TEXTURE1);
	// save current settings:
	int view[4];
	glGetIntegerv(GL_VIEWPORT, view);

	// New viewport
	glViewport(0, 0, WNMR_W, WNMR_H);

	// Setup the WaterNormalMapTexture and buffer
	WaterNormalMapTexture = createEmptyTexture(WNMR_W, WNMR_H);
	WaterNormaMapFrameBuffer.createAndBind();
	WaterNormaMapFrameBuffer.attachTexture(0, WaterNormalMapTexture, WNMR_W, WNMR_H);
	WaterNormaMapFrameBuffer.setDrwaBuff();
	WaterNormaMapFrameBuffer.check();

	// Unbind all the textures and buffers
	FrameBufObj::resetBinding();
	
	// Reset Viewport to previouse settings
	glViewport(view[0], view[1], view[2], view[3]);
}

GLuint WaterRenderer::createEmptyTexture(GLuint w, GLuint h)
{
	// Generate texture ID
	GLuint texId;
	glGenTextures(1, &texId);

	// Set appropriate values and generate empty image
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

	return texId;
}

void WaterRenderer::initSquareGeometry(GLuint* buffId)
{
	assert(buffId && "vao cannot be null");

	// Triangle strip
	static const float quadData[] =
	{
		-1.0f, -1.0f, 0.0f, // position
		0.0f, 0.0f,         // texture coordinates

		-1.0f, 1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
	};

	// Fill the apropriate buffers
	const GLsizei STRIDE = sizeof(float) * 5;
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);

	glGenVertexArrays(1, buffId);
	glBindVertexArray(*buffId);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE, reinterpret_cast<const void *>(sizeof(float) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WaterRenderer::drawSquare(GLuint buffId)
{
	// Bind the sqare buffer and draw the array
	glBindVertexArray(buffId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}