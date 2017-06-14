#include "WaterRenderer.hpp"
#include "WaterShader.hpp"
#include "OBJLoader.hpp"
#include <glm/gtc/matrix_transform.inl>
#include "GLFW/glfw3.h"

using namespace glm;

WaterRenderer::WaterRenderer(vec3& lightPosition) :
	_waterShader(WaterShader("waterShader")),
	_waterHeightShader(WaterHeightShader("waterHeightMapShader")),
	_waterNormalShader(WaterNormalShader("waterNormalMapShader")),
	_waterAddDropShader(WaterAddDropShader("waterAddDropShader"))
{
	// Initialize the three different maps
	//  - the plane
	initWaterPlane();
	//  - the height map
	initWaterHeightMaps();
	//  - the normal map
	initWaterNormalMap();

	// Initializes the shaders
	//  - the water shader, wich is responsible to draw the water and its colours
	_waterShader.use();
	_waterShader.loadWaterHeightMap(0);
	_waterShader.loadWaterNormalMap(1);
	_waterShader.loadLightPosition(lightPosition);
	vec4 waterColor = vec4(112.0 / 255.0, 143.0 / 255.0, 184.0 / 255.0, 1.0);
	_waterShader.loadWaterColor(waterColor);
	_waterShader.stop();

	//  - the water height shader, wich will update the height map
	_waterHeightShader.use();
	_waterHeightShader.loadODWHMR(1.0f / WaterHeightMapResolution_Width);
	_waterHeightShader.stop();

	//  - the water normal shader, wich will update the normal map
	_waterNormalShader.use();
	_waterNormalShader.loadODWNMR(1.0f / WaterNormalMapResolution_Width);
	_waterNormalShader.loadWMSDWNMRM2(2.0f / WaterNormalMapResolution_Width * 2);
	_waterNormalShader.stop();

	// Setupt the square to update the maps
	initSquareGeometry(&_squareBuffId);
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

		// Set viewport to exactly the height and width of the WaterHeightMap resolution
		glViewport(0, 0, WaterHeightMapResolution_Width, WaterHeightMapResolution_Height);
		
		// Get the next id for the water height buffer
		GLuint nextId = (_waterHeightMapId + 1) % 2;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		// Bind the water height map, use the HeightMapShader and draw
		// a square to update the water height map and unbind everything
		_waterHeightMapFrameBuffers[nextId].bind();
		_waterHeightShader.use();
		_waterHeightMapFrameBuffers[_waterHeightMapId].bindColourTarget(0);

		drawSquare(_squareBuffId);
		_waterHeightShader.stop();

		FrameBufObj::resetBinding();

		// Set the next buffer
		_waterHeightMapId = nextId;

		// Set viewport to exactly the height and width of the WaterNormalMap resolution
		glViewport(0, 0, WaterNormalMapResolution_Width, WaterNormalMapResolution_Height);

		// Bind the WaterNormalMap to write into, use the WaterNormalShader and bind
		// the current heightMap to calculate the normals and then unbind everything
		_waterNormalMapFrameBuffer.bind();
		_waterNormalShader.use();
		_waterHeightMapFrameBuffers[_waterHeightMapId].bindColourTarget(0);
		drawSquare(_squareBuffId);
		_waterNormalShader.stop();

		FrameBufObj::resetBinding();

		// reset the Viewport to the previouse values
		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}

	// Actual rendering
	// use the water shader and pass in the appropriate matrices
	_waterShader.use();
	_waterShader.loadProjectionMatrix(projection);
	_waterShader.loadModelMatrix(waterModel);
	_waterShader.loadViewMatrix(view);
	_waterShader.loadCameraPosition(cameraPosition);

	// Bind the WaterHeightMap and the WaterNormalMap for the shaders use
	glActiveTexture(GL_TEXTURE0);
	_waterHeightMapFrameBuffers[_waterHeightMapId].bindColourTarget(0);
	glActiveTexture(GL_TEXTURE1);
	_waterNormalMapFrameBuffer.bindColourTarget(0);

	// Draw the actual plane
	glBindVertexArray(_waterPlaneBuffId);
	glDrawArrays(GL_TRIANGLES, 0, _triangleVerticesCount);
	glBindVertexArray(0);
	_waterShader.stop();

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
		glViewport(0, 0, WaterMeshResolution_Width, WaterMeshResolution_Width);

		// Calculate the next ID for the WaterHeightMap
		GLuint nextId = (_waterHeightMapId + 1) % 2;
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		// Bind the next WaterHeightMap to write into
		_waterHeightMapFrameBuffers[nextId].bind();
		// Use the WaterDropShader and pass in the apropriate values
		_waterAddDropShader.use();
		_waterAddDropShader.loadDropRadius(0.05);
		vec2 dropPosition = vec2(0.5, 0.5);
		_waterAddDropShader.loadPosition(dropPosition);
		// Load the current WaterHeightMap to read from
		_waterHeightMapFrameBuffers[_waterHeightMapId].bindColourTarget(0);

		// Draw a square on the screen to update the WaterHeightMap
		drawSquare(_squareBuffId);

		// Unbind Shader and textures
		_waterAddDropShader.stop();
		FrameBufObj::resetBinding();

		// Change WaterHeightMap buffers
		_waterHeightMapId = nextId;

		// Reset Viewprot to the original values
		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}
}

void WaterRenderer::initWaterPlane()
{
	int WMRP1 = WaterMeshResolution_Width + 1;
	vec3 *Vertices = new vec3[WMRP1 * WMRP1];
	
	float WMSDWMR = 2.0f / static_cast<float>(WaterMeshResolution_Width);
	
	// Create triangel points
	for (int y = 0; y <= WaterMeshResolution_Width; y++)
	{
		for (int x = 0; x <= WaterMeshResolution_Width; x++)
		{
			Vertices[WMRP1 * y + x].x = x * WMSDWMR - 1.0f;
			Vertices[WMRP1 * y + x].y = 0.0f;
			Vertices[WMRP1 * y + x].z = 1.0f - y * WMSDWMR;
		}
	}
	
	std::vector<GLfloat> triangles;

	// draw the actual triangles
	for (int y = 0; y < WaterMeshResolution_Width; y++)
	{
		int yp1 = y + 1;
	
		for (int x = 0; x < WaterMeshResolution_Width; x++)
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
	glGenVertexArrays(1, &_waterPlaneBuffId);
	glBindVertexArray(_waterPlaneBuffId);

	GLuint t;
	glGenBuffers(1, &t);
	glBindBuffer(GL_ARRAY_BUFFER, t);
	glBufferData(GL_ARRAY_BUFFER, triangles.size() * 4, triangles.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
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
	glViewport(0, 0, WaterHeightMapResolution_Width, WaterHeightMapResolution_Height);

	for (int i = 0; i < 2; i++)
	{
		// Setup the WaterHeightMapTextures and buffers
		_waterHeightMapTextures[i] = createEmptyTexture(WaterHeightMapResolution_Width, WaterHeightMapResolution_Height);

		_waterHeightMapFrameBuffers[i].createAndBind();
		_waterHeightMapFrameBuffers[i].attachTexture(0, _waterHeightMapTextures[i], WaterHeightMapResolution_Width, WaterHeightMapResolution_Height);
		_waterHeightMapFrameBuffers[i].setDrwaBuff();
		_waterHeightMapFrameBuffers[i].check();
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
	glViewport(0, 0, WaterNormalMapResolution_Width, WaterNormalMapResolution_Height);

	// Setup the WaterNormalMapTexture and buffer
	_waterNormalMapTexture = createEmptyTexture(WaterNormalMapResolution_Width, WaterNormalMapResolution_Height);
	_waterNormalMapFrameBuffer.createAndBind();
	_waterNormalMapFrameBuffer.attachTexture(0, _waterNormalMapTexture, WaterNormalMapResolution_Width, WaterNormalMapResolution_Height);
	_waterNormalMapFrameBuffer.setDrwaBuff();
	_waterNormalMapFrameBuffer.check();

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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, nullptr);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE, nullptr);
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