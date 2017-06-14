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
	initTriangles();
	initWaterHeightMaps();
	initWaterNormalMap();

	m_waterShader.use();
	m_waterShader.loadWaterHightMap(0);
	m_waterShader.loadWaterNormalMap(1);
	m_waterShader.loadLightPosition(lightPosition);
	vec4 waterColor = vec4(112.0 / 255.0, 143.0 / 255.0, 184.0 / 255.0, 1.0);
	m_waterShader.loadWaterColor(waterColor);
	m_waterShader.stop();

	m_waterHightShader.use();
	m_waterHightShader.loadODWHMR(1.0f / WHMR_W);
	m_waterHightShader.stop();

	m_waterNormalShader.use();
	m_waterNormalShader.loadODWNMR(1.0f / WNMR_W);
	m_waterNormalShader.loadWMSDWNMRM2(2.0f / WNMR_W * 2);
	m_waterNormalShader.stop();

	glGenFramebuffersEXT(1, &FBO);

	initQuadGeometry(&mQuadVAO);
}

WaterRenderer::~WaterRenderer()
{
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
		
		//GLuint nextId = WHMID;
		GLuint nextId = (WHMID + 1) % 2;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		WaterHeightMapFrameBuffers[nextId].bind();
		m_waterHightShader.use();
		//WaterHeightMapsFrameBuffer[nextId].bindColorTargetAsTexture(0);
		WaterHeightMapFrameBuffers[WHMID].bindColorTargetAsTexture(0);

		drawQuad(mQuadVAO);
		m_waterHightShader.stop();

		FrameBuffer::bindSystemFrameBuffer();
		glBindTexture(GL_TEXTURE_2D, 0);

		WHMID = nextId;
		//++WHMID %= 2;

		// Set viewport to exactly the hight and width of the WaterNormalMap resolution
		glViewport(0, 0, WNMR_W, WNMR_H);

		WaterNormaMapFrameBuffer.bind();
		m_waterNormalShader.use();
		WaterHeightMapFrameBuffers[WHMID].bindColorTargetAsTexture(0);
		drawQuad(mQuadVAO);
		m_waterNormalShader.stop();

		FrameBuffer::bindSystemFrameBuffer();
		glBindTexture(GL_TEXTURE_2D, 0);

		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}

	//WaterHeightMapsFrameBuffer[WHMID].bind();
	m_waterShader.use();
	m_waterShader.loadProjectionMatrix(projection);
	m_waterShader.loadModelMatrix(waterModel);
	m_waterShader.loadViewMatrix(view);
	m_waterShader.loadCameraPosition(cameraPosition);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, WaterHeightMapTextures[WHMID]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, WaterNormalMapTexture);

	glBindVertexArray(WaterVBO);
	glDrawArrays(GL_TRIANGLES, 0, QuadsVerticesCount);
	glBindVertexArray(0);
	m_waterShader.stop();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterRenderer::addDrop()
{
	//if (x >= -1.0f && x <= 1.0f && y >= -1.0f && y <= 1.0f)
	{
		int origViewport[4];
		glGetIntegerv(GL_VIEWPORT, origViewport);
		glViewport(0, 0, WMR, WMR);

		GLuint nextId = (WHMID + 1) % 2;
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		WaterHeightMapFrameBuffers[nextId].bind();
		m_waterAddDropShader.use();
		m_waterAddDropShader.loadDropRadius(0.05);
		vec2 p = vec2(0.5, 0.5);
		m_waterAddDropShader.loadPosition(p);
		WaterHeightMapFrameBuffers[WHMID].bindColorTargetAsTexture(0);

		drawQuad(mQuadVAO);
		m_waterAddDropShader.stop();
		FrameBuffer::bindSystemFrameBuffer();
		glBindTexture(GL_TEXTURE_2D, 0);

		WHMID = nextId;

		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}
}

void WaterRenderer::initTriangles()
{
	int WMRP1 = WMR + 1;
	vec3 *Vertices = new vec3[WMRP1 * WMRP1];
	
	float WMSDWMR = 2.0f / static_cast<float>(WMR);
	
	for (int y = 0; y <= WMR; y++)
	{
		for (int x = 0; x <= WMR; x++)
		{
			Vertices[WMRP1 * y + x].x = x * WMSDWMR - 1.0f;
			Vertices[WMRP1 * y + x].y = 0.0f;
			Vertices[WMRP1 * y + x].z = 1.0f - y * WMSDWMR;
		}
	}
	
	std::vector<GLfloat> quads;

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
	
			quads.push_back(Vertices[a].x);
			quads.push_back(Vertices[a].y);
			quads.push_back(Vertices[a].z);
			quads.push_back(Vertices[b].x);
			quads.push_back(Vertices[b].y);
			quads.push_back(Vertices[b].z);
			quads.push_back(Vertices[c].x);
			quads.push_back(Vertices[c].y);
			quads.push_back(Vertices[c].z);
	
			quads.push_back(Vertices[d].x);
			quads.push_back(Vertices[d].y);
			quads.push_back(Vertices[d].z);
			quads.push_back(Vertices[a].x);
			quads.push_back(Vertices[a].y);
			quads.push_back(Vertices[a].z);
			quads.push_back(Vertices[c].x);
			quads.push_back(Vertices[c].y);
			quads.push_back(Vertices[c].z);
		}
	}

	glGenVertexArrays(1, &WaterVBO);
	glBindVertexArray(WaterVBO);

	GLuint t;
	glGenBuffers(1, &t);
	glBindBuffer(GL_ARRAY_BUFFER, t);
	glBufferData(GL_ARRAY_BUFFER, quads.size() * 4, quads.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	QuadsVerticesCount = quads.size() / 3;
	
	quads.clear();
	
	delete[] Vertices;
}

void WaterRenderer::initWaterHeightMaps()
{
	glActiveTexture(GL_TEXTURE0);

	// save current settings:
	int view[4];
	glGetIntegerv(GL_VIEWPORT, view);
	float col[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, col);

	// new viewport:
	glViewport(0, 0, WHMR_W, WHMR_H);

	for (int i = 0; i < 2; i++)
	{
		WaterHeightMapTextures[i] = createEmptyTexture2D(WHMR_W, WHMR_H);

		WaterHeightMapFrameBuffers[i].createAndBind();
		WaterHeightMapFrameBuffers[i].attachTextureAsColorTarget(0, WaterHeightMapTextures[i], WHMR_W, WHMR_H);
		WaterHeightMapFrameBuffers[i].setDrawBuffers();
		WaterHeightMapFrameBuffers[i].check();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// reset to previouse settings
	FrameBuffer::bindSystemFrameBuffer();
	glViewport(view[0], view[1], view[2], view[3]);
	glClearColor(col[0], col[1], col[2], col[3]);
}

void WaterRenderer::initWaterNormalMap()
{
	glActiveTexture(GL_TEXTURE1);
	// save current settings:
	int view[4];
	glGetIntegerv(GL_VIEWPORT, view);
	float col[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, col);

	glViewport(0, 0, WNMR_W, WNMR_H);

	WaterNormalMapTexture = createEmptyTexture2D(WNMR_W, WNMR_H);
	WaterNormaMapFrameBuffer.createAndBind();
	WaterNormaMapFrameBuffer.attachTextureAsColorTarget(0, WaterNormalMapTexture, WNMR_W, WNMR_H);
	WaterNormaMapFrameBuffer.setDrawBuffers();
	WaterNormaMapFrameBuffer.check();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// reset to previouse settings
	FrameBuffer::bindSystemFrameBuffer();
	glViewport(view[0], view[1], view[2], view[3]);
	glClearColor(col[0], col[1], col[2], col[3]);
}

GLuint WaterRenderer::createEmptyTexture2D(GLuint w, GLuint h)
{
	GLuint texId;
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

	return texId;
}

void WaterRenderer::initQuadGeometry(GLuint* vao)
{
	assert(vao && "vao cannot be null");

	// triangle strip
	static const float quadData[] =
	{
		-1.0f, -1.0f, 0.0f, // pos
		0.0f, 0.0f,         // tex

		-1.0f, 1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
	};

	const GLsizei STRIDE = sizeof(float) * 5;

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);

	//
	// VAO setup
	//
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE, (const void *)(sizeof(float) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WaterRenderer::drawQuad(GLuint vao)
{
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}