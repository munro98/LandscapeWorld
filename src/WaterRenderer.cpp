#include "WaterRenderer.hpp"
#include <glm/gtc/matrix_transform.inl>
#include "GLFW/glfw3.h"

using namespace glm;

WaterRenderer::WaterRenderer(mat4& projection, vec3& lightPosition) :
	_phongShader(PhongShader("phongShader")),
	_waterHeightShader(WaterHeightShader("waterHeightMapShader")),
	_waterNormalShader(WaterNormalShader("waterNormalMapShader")),
	_waterAddDropShader(WaterAddDropShader("waterAddDropShader"))
{
	// Initialize the three different maps
	initWaterPlane();
	initWaterHeightMaps();
	initWaterNormalMap();

	// Initializes the shaders
	_waterShaders.push_back(WaterShader("waterShader"));
	initWaterShader(_waterShaders[0], projection, lightPosition);
	_waterShaders.push_back(WaterShader("waterShader", "waterShaderNoColor"));
	initWaterShader(_waterShaders[1], projection, lightPosition);
	initWaterHeightMapShader();
	initWaterNormalMapShader();
	initPhongShader(projection, lightPosition);

	// Setupt the square to update the maps
	initSquareGeometry(&_squareBuffId);

	// Init Bathtub mesh
	_bathtubMesh = OBJLoader::loadObjModel("bathtub");
}

WaterRenderer::~WaterRenderer()
{
	//for(int i = 0; i < 2; i++)
	//{
	//	WaterHeightMapFrameBuffers[i].destroy();
	//}

	//WaterNormaMapFrameBuffer.destroy();
}

void WaterRenderer::render(mat4& view, mat4& model, mat4& projection, vec3& cameraPosition, float dropSize, float rainIntensity)
{
	mat4 waterModel;// = model;
	waterModel = translate(model, vec3(0, 1, 0));
	waterModel = scale(waterModel, vec3(2, 2, 2));

	double currentTime = glfwGetTime();
	float timeDiff = _lastTime == 0 ? 0 : currentTime - _lastTime;
	_rainDiff += timeDiff;
	_updateDiff += timeDiff;
	_lastTime = currentTime;

	if(rainIntensity > 0.001 && _rainDiff > (1 - rainIntensity))
	{
		_rainDiff = 0;
		float x = (rand() % 101) / 100.0;
		float y = (rand() % 101) / 100.0;
		addDrop(x, y, dropSize);
	}

	if (_updateDiff > 0.016)
	{
		_updateDiff = 0;

		// Save current viewport settings
		int origViewport[4];
		glGetIntegerv(GL_VIEWPORT, origViewport);

		updateWaterHeightMap();

		updateWaterNormalMap();

		// reset the Viewport to the previouse values
		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
	}

	// Actual rendering
	// use the water shader and pass in the appropriate matrices
	renderWaterPlain(_waterShaders[_waterShaderID], view, projection, cameraPosition, waterModel);

	waterModel = translate(waterModel, vec3(0, -1.1, 0));
	renderBathtub(view, waterModel, projection, cameraPosition);
}

void WaterRenderer::addDrop(float x, float y, float dropRadius)
{
	if (x >= 0 && x <= 1.0f && y >= 0 && y <= 1.0f)
	{
		// Save the current viewport
		int origViewport[4];
		glGetIntegerv(GL_VIEWPORT, origViewport);

		// Set Viewport to with and height of the Water Map
		glViewport(0, 0, WaterMeshResolution_Width, WaterMeshResolution_Height);

		// Calculate the next ID for the WaterHeightMap
		GLuint nextId = (_waterHeightMapId + 1) % 2;
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		// Bind the next WaterHeightMap to write into
		_waterHeightMapFrameBuffers[nextId].bind();

		// Use the WaterDropShader and pass in the apropriate values
		_waterAddDropShader.use();
		_waterAddDropShader.loadDropRadius(0.02 * dropRadius);
		vec2 dropPosition = vec2(x, 1.0f-y);
		_waterAddDropShader.loadDropPosition(dropPosition);

		// Load the current WaterHeightMap to read from
		_waterHeightMapFrameBuffers[_waterHeightMapId].bindColourTarget();

		// Draw a square on the screen to update the WaterHeightMap
		drawSquare(_squareBuffId);

		// Unbind Shader and textures
		_waterAddDropShader.stop();
		FrameBufObj::resetBinding();

		// Change WaterHeightMap buffers
		_waterHeightMapId = nextId;

		// Reset Viewprot to the original values
		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
	}
}

void WaterRenderer::showColor(bool showColor)
{
	_waterShaderID = showColor ? 0 : 1;
}

void WaterRenderer::initWaterPlane()
{
	int wmrPlusOne = WaterMeshResolution_Width + 1;
	vec3 *Vertices = new vec3[wmrPlusOne * wmrPlusOne];
	
	float stepSize = 2.0f / static_cast<float>(WaterMeshResolution_Width);
	
	// Create triangel points
	for (int y = 0; y <= WaterMeshResolution_Width; y++)
	{
		for (int x = 0; x <= WaterMeshResolution_Height; x++)
		{
			Vertices[wmrPlusOne * y + x].x = x * stepSize - 1.0f;
			Vertices[wmrPlusOne * y + x].y = 0.0f;
			Vertices[wmrPlusOne * y + x].z = 1.0f - y * stepSize;
		}
	}
	
	std::vector<GLfloat> triangles;

	// draw the actual triangles
	for (int y = 0; y < WaterMeshResolution_Width; y++)
	{
		int yPlusOne = y + 1;
	
		for (int x = 0; x < WaterMeshResolution_Height; x++)
		{
			int xPlusOne = x + 1;
	
			int a = wmrPlusOne * y + x;
			int b = wmrPlusOne * y + xPlusOne;
			int c = wmrPlusOne * yPlusOne + xPlusOne;
			int d = wmrPlusOne * yPlusOne + x;
	
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

void WaterRenderer::updateWaterHeightMap()
{
	// Set viewport to exactly the height and width of the WaterHeightMap resolution
	glViewport(0, 0, WaterHeightMapResolution_Width, WaterHeightMapResolution_Height);

	//// Get the next id for the water height buffer
	GLuint nextId = (_waterHeightMapId + 1) % 2;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// Bind the water height map, use the HeightMapShader and draw
	// a square to update the water height map and unbind everything
	_waterHeightMapFrameBuffers[nextId].bind();
	_waterHeightShader.use();
	_waterHeightMapFrameBuffers[_waterHeightMapId].bindColourTarget();

	drawSquare(_squareBuffId);
	_waterHeightShader.stop();

	FrameBufObj::resetBinding();

	// Set the next buffer
	_waterHeightMapId = nextId;
}

void WaterRenderer::updateWaterNormalMap()
{
	// Set viewport to exactly the height and width of the WaterNormalMap resolution
	glViewport(0, 0, WaterNormalMapResolution_Width, WaterNormalMapResolution_Height);

	// Bind the WaterNormalMap to write into, use the WaterNormalShader and bind
	// the current heightMap to calculate the normals and then unbind everything
	_waterNormalMapFrameBuffer.bind();
	_waterNormalShader.use();
	_waterHeightMapFrameBuffers[_waterHeightMapId].bindColourTarget();
	drawSquare(_squareBuffId);
	_waterNormalShader.stop();

	FrameBufObj::resetBinding();
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
		_waterHeightMapFrameBuffers[i].createAndBind(_waterHeightMapTextures[i]);
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
	_waterNormalMapFrameBuffer.createAndBind(_waterNormalMapTexture);

	// Unbind all the textures and buffers
	FrameBufObj::resetBinding();
	
	// Reset Viewport to previouse settings
	glViewport(view[0], view[1], view[2], view[3]);
}

void WaterRenderer::initWaterShader(WaterShader& shader, mat4& projection, vec3& lightPosition)
{
	//  - the water shader, wich is responsible to draw the water and its colours
	shader.use();
	shader.loadWaterHeightMap(0);
	shader.loadWaterNormalMap(1);
	shader.loadLightPosition(lightPosition);
	shader.loadProjectionMatrix(projection);
	vec4 waterColor = vec4(112.0 / 255.0, 143.0 / 255.0, 184.0 / 255.0, 1);
	shader.loadWaterColour(waterColor);
	shader.loadShowColour(true);
	shader.stop();
}

void WaterRenderer::initWaterHeightMapShader()
{
	//  - the water height shader, wich will update the height map
	_waterHeightShader.use();
	_waterHeightShader.loadWaterHeightMapDistWidth(1.0f / WaterHeightMapResolution_Width);
	_waterHeightShader.loadWaterHeightMapDistHeight(1.0f / WaterHeightMapResolution_Height);
	_waterHeightShader.loadSin45(sqrt(2.0f) / 2);
	_waterHeightShader.loadAttenuation(0.99);
	_waterHeightShader.stop();
}

void WaterRenderer::initWaterNormalMapShader()
{
	//  - the water normal shader, wich will update the normal map
	_waterNormalShader.use();
	_waterNormalShader.loadwaterNormMapDistWidth(1.0 / WaterNormalMapResolution_Width);
	_waterNormalShader.loadwaterNormMapDistHeight(1.0 / WaterNormalMapResolution_Height);
	_waterNormalShader.loadDefaultHeight(4.0 / ((WaterNormalMapResolution_Width + WaterNormalMapResolution_Height) / 2.0));
	_waterNormalShader.stop();
}

void WaterRenderer::initPhongShader(mat4& projection, vec3& lightPosition)
{
	// Init bathtub Shader
	_phongShader.use();
	_phongShader.loadProjectionMatrix(projection);
	_phongShader.loadLightDirection(lightPosition);
	auto matAmbient = vec3(0.2, 0.2, 0.2);
	auto matDiffuse = vec3(0.8, 0.8, 0.8);
	auto matSpecular = vec3(0.3, 0.3, 0.3);
	auto lightAmbient = vec3(0.3, 0.3, 0.3);
	auto lightDiffuse = vec3(0.6, 0.6, 0.6);
	auto lightSpecular = vec3(0.8, 0.8, 0.8);
	_phongShader.loadMaterialAmbient(matAmbient);
	_phongShader.loadMaterialDiffuse(matDiffuse);
	_phongShader.loadMaterialSpecular(matSpecular);
	_phongShader.loadMaterialShininess(0.088 * 128);
	_phongShader.loadLightAmbient(lightAmbient);
	_phongShader.loadLightDiffuse(lightDiffuse);
	_phongShader.loadLightSpecular(lightSpecular);
	_phongShader.stop();
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

void WaterRenderer::renderWaterPlain(WaterShader& shader, mat4& view, mat4& projection, vec3& cameraPosition, mat4 waterModel)
{
	shader.use();
	shader.loadProjectionMatrix(projection);
	shader.loadModelMatrix(waterModel);
	shader.loadViewMatrix(view);
	shader.loadCameraPosition(cameraPosition);

	// Bind the WaterHeightMap and the WaterNormalMap for the shaders use
	glActiveTexture(GL_TEXTURE0);
	_waterHeightMapFrameBuffers[_waterHeightMapId].bindColourTarget();
	glActiveTexture(GL_TEXTURE1);
	_waterNormalMapFrameBuffer.bindColourTarget();

	// Draw the actual plane
	glBindVertexArray(_waterPlaneBuffId);
	glDrawArrays(GL_TRIANGLES, 0, _triangleVerticesCount);
	glBindVertexArray(0);
	shader.stop();

	// Unbind everything
	glActiveTexture(GL_TEXTURE1);
	FrameBufObj::resetBinding();
	glActiveTexture(GL_TEXTURE0);
	FrameBufObj::resetBinding();
}

void WaterRenderer::renderBathtub(mat4& view, mat4& model, mat4& projection, vec3& cameraPosition)
{
	_phongShader.use();
	//Update uniforms
	_phongShader.loadModelMatrix(model);
	_phongShader.loadViewMatrix(view);
	_phongShader.loadProjectionMatrix(projection);
	_phongShader.loadCameraPosition(cameraPosition);

	glBindVertexArray(_bathtubMesh->getVaoID());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, _bathtubMesh->getVertexCount(), GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}