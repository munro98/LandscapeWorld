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
	GLint gl_max_texture_max_anisotropy_ext = 0;
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &gl_max_texture_max_anisotropy_ext);

	initTriangles();
	initWaterHeightMaps(gl_max_texture_max_anisotropy_ext);
	initWaterNormalMap(gl_max_texture_max_anisotropy_ext);

	m_waterShader.use();
	m_waterShader.loadWaterHightMap(0);
	m_waterShader.loadWaterNormalMap(1);
	m_waterShader.loadLightPosition(lightPosition);
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

	//glGenFramebuffers(1, &mFboId);
	//glBindFramebuffer(GL_FRAMEBUFFER, mFboId);

	//m_mesh = OBJLoader::loadObjModel("waterplane");
	//m_texture = Loader::loadTexture("waterNormalMap");
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
	
	//glm::mat4 m = glm::mat4();
	//glm::mat4 m2 = glm::mat4();
	//glm::mat4 m3 = glm::mat4();
	//m3 = glm::ortho(-1, 1, 1, -1, -1, 100);
	//m = glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	////m = glm::translate(m, glm::vec3(0, 0, -10));
	//m2 = glm::rotate(m2, -45.0f, glm::vec3(1, 0, 0));
	////m = projection * m;	//m_waterShader.loadProjectionMatrix(m3);
	//m_waterShader.loadModelMatrix(m2);
	//m_waterShader.loadViewMatrix(m);

	double currentTime = glfwGetTime();
	_diff += _lastTime == 0 ? 0 : currentTime - _lastTime;
	_lastTime = currentTime;

	if (_diff > 0.016)
	{
		_diff = 0;

		int origViewport[4];
		glGetIntegerv(GL_VIEWPORT, origViewport);

		glViewport(0, 0, WHMR_W, WHMR_H);
		
		//GLuint whmid = (WHMID + 1) % 2;

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, WaterHeightMapsTexture[whmid], 0);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);

		//m_waterHightShader.use();
		//glBindTexture(GL_TEXTURE_2D, WaterHeightMapsTexture[WHMID]);
		//glBegin(GL_QUADS);
		//glVertex2f(0.0f, 0.0f);
		//glVertex2f(1.0f, 0.0f);
		//glVertex2f(1.0f, 1.0f);
		//glVertex2f(0.0f, 1.0f);
		//glEnd();
		//glBindTexture(GL_TEXTURE_2D, 0);
		//m_waterHightShader.stop();

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		//glBindTexture(GL_TEXTURE_2D, WaterHeightMapsTexture[whmid]);
		//glGenerateMipmapEXT(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, 0);

		//++WHMID %= 2;

		//// update water normal map
		//glViewport(0, 0, WNMR_W, WNMR_H);
		//
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, WaterNormalMap, 0);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);

		//m_waterNormalShader.use();
		//glBindTexture(GL_TEXTURE_2D, WaterHeightMapsTexture[WHMID]);
		//glBegin(GL_QUADS);
		//glVertex2f(0.0f, 0.0f);
		//glVertex2f(1.0f, 0.0f);
		//glVertex2f(1.0f, 1.0f);
		//glVertex2f(0.0f, 1.0f);
		//glEnd();
		//glBindTexture(GL_TEXTURE_2D, 0);
		//m_waterNormalShader.stop();

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		//glBindTexture(GL_TEXTURE_2D, WaterNormalMap);
		//glGenerateMipmapEXT(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}

	m_waterShader.use();
	m_waterShader.loadProjectionMatrix(projection);
	m_waterShader.loadModelMatrix(waterModel);
	m_waterShader.loadViewMatrix(view);
	m_waterShader.loadCameraPosition(cameraPosition);

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[0]);
	glBindTexture(GL_TEXTURE_2D, WaterHeightMapsTexture[WHMID]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, WaterNormalMap);

	glBindVertexArray(WaterVBO);
	glDrawArrays(GL_TRIANGLES, 0, QuadsVerticesCount);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
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

		GLuint nextID = 1 - WHMID;
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		WaterHeightMapsFrameBuffer[nextID].bind();
		m_waterAddDropShader.use();
		m_waterAddDropShader.loadDropRadius(0.1);
		vec2 p = vec2(0.5, 0.5);
		m_waterAddDropShader.loadPosition(p);
		WaterHeightMapsFrameBuffer[nextID].bindColorTargetAsTexture(0);

		drawQuad(mQuadVAO);
		m_waterAddDropShader.stop();
		FrameBuffer::bindSystemFrameBuffer();
		glBindTexture(GL_TEXTURE_2D, 0);

		++WHMID %= 2;

		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}
}

//void WaterRenderer::addDrop()
//{
//	//if (x >= -1.0f && x <= 1.0f && y >= -1.0f && y <= 1.0f)
//	{
//		//glViewport(0, 0, WMR, WMR);
//
//		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
//		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, WaterHeightMaps[(WHMID + 1) % 2], 0);
//		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);
//
//		m_waterAddDropShader.use();
//		glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[WHMID]);
//		//glUniform1f(WaterAddDropProgram.UniformLocations[0], DropRadius);
//		//glUniform2fv(WaterAddDropProgram.UniformLocations[1], 1, &vec2(x * 0.5f + 0.5f, 0.5f - y * 0.5f));
//		//glUniform1f(glGetUniformLocation(_waterDropShader, "DropRadius"), 0.05);
//		//glUniform2fv(glGetUniformLocation(_waterDropShader, "Position"), 1, &vec2(0.5f, 0.5f));
//		m_waterAddDropShader.loadDropRadius(0.05);
//		vec2 p = vec2(0.5, 0.5);
//		m_waterAddDropShader.loadPosition(p);
//		glBegin(GL_QUADS);
//		glVertex2f(0.0f, 0.0f);
//		glVertex2f(1.0f, 0.0f);
//		glVertex2f(1.0f, 1.0f);
//		glVertex2f(0.0f, 1.0f);
//		glEnd();
//		glBindTexture(GL_TEXTURE_2D, 0);
//		m_waterAddDropShader.stop();
//
//		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//
//		++WHMID %= 2;
//	}
//}

//void WaterRenderer::render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition)
//{
//	//m_waterShader.use();
//	//Update uniforms
//	//m_waterShader.loadModelMatrix(model);
//	//m_waterShader.loadViewMatrix(view);
//	//m_waterShader.loadProjectionMatrix(projection);
//	//m_waterShader.loadCameraPosition(cameraPosition);
//
//	//glViewport(0, 0, 640, 480);
//	//glMatrixMode(GL_PROJECTION);
//	//glLoadMatrixf(&projection[0][0]);
//	//glMatrixMode(GL_MODELVIEW);
//	//glLoadMatrixf(&(view * model)[0][0]);
//	//GLUquadric* quad = gluNewQuadric();
//	//glColor3f(1, 0, 0);
//	//gluSphere(quad, 10, 20, 20);
//
//	// Set up the view part of the model view matrix
//	//glUseProgram(_waterShader);
//	//glScalef(5, 5, 5);
//	m_waterShader.use();
//	//m_waterShader.loadCameraPosition(cameraPosition);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[0]);
//	//glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[WHMID]);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, WaterNormalMap);
//
//	glBindBuffer(GL_ARRAY_BUFFER, WaterVBO);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
//	glDrawArrays(GL_TRIANGLES, 0, QuadsVerticesCount);
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	m_waterShader.stop();
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}

void WaterRenderer::initTriangles()
{
	// Send simple triangle to GPU
	//std::vector<glm::vec3> points = std::vector<glm::vec3>();
	//points.push_back(glm::vec3(-0.5, -0.5, 0));
	//points.push_back(glm::vec3(0.5, -0.5, 0));
	//points.push_back(glm::vec3(0.0, -0.5, 0));
	//std::vector<GLfloat> points = {
	//	-0.5f,-0.5f,0.0f,
	//	0.5f,-0.5f,0.0f,
	//	0.0f,0.5f,0.0f
	//};
	//static GLfloat vertices[] = {
	//	-0.5f,-0.5f,0.0f,
	//	0.5f,-0.5f,0.0f,
	//	0.0f,0.5f,0.0f
	//};

	//int t2 = sizeof(vertices);
	//t2 = sizeof(points);

	//points.clear();

	int WMRP1 = WMR + 1;
	vec3 *Vertices = new vec3[WMRP1 * WMRP1];
	
	float WMSDWMR = 2.0f / (float)WMR;
	
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

	//quads.push_back(Vertices[1].x);
	//quads.push_back(Vertices[1].y);
	//quads.push_back(Vertices[1].z);
	//quads.push_back(Vertices[2000].x);
	//quads.push_back(Vertices[2000].y);
	//quads.push_back(Vertices[2000].z);
	//quads.push_back(-Vertices[16000].x);
	//quads.push_back(Vertices[16000].y);
	//quads.push_back(-Vertices[16000].z);
	//quads.push_back(-1);
	//quads.push_back(1);
	//quads.push_back(0.0f);
	//quads.push_back(0.01);
	//quads.push_back(0.76);
	//quads.push_back(0.0f);
	//quads.push_back(-0.93);
	//quads.push_back(-0.93);
	//quads.push_back(0.0f);

	//quads.push_back(-1);
	//quads.push_back(0.0f);
	//quads.push_back(-1);
	//quads.push_back(1);
	//quads.push_back(0.0f);
	//quads.push_back(-1);
	//quads.push_back(-1);
	//quads.push_back(0.0f);
	//quads.push_back(1);
	//quads.push_back(1);
	//quads.push_back(0.0f);
	//quads.push_back(1);
	//quads.push_back(-1);
	//quads.push_back(0.0f);
	//quads.push_back(-1);
	//quads.push_back(-1);
	//quads.push_back(0.0f);
	//quads.push_back(1);
	
	//quads.push_back(-1);
	//quads.push_back(-1);
	//quads.push_back(0.0f);
	//quads.push_back(1);
	//quads.push_back(-1);
	//quads.push_back(0.0f);
	//quads.push_back(0);
	//quads.push_back(1);
	//quads.push_back(0.0f);

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

//void WaterRenderer::initTriangles()
//{
//	glGenVertexArrays(1, &WaterVBO);
//	glBindVertexArray(WaterVBO);
//
//	GLuint t; 
//	glGenBuffers(1, &t);
//
//	int WMRP1 = WMR + 1;
//
//	glm::vec3 *Vertices = new glm::vec3[WMRP1 * WMRP1];
//
//	float WMSDWMR = 2.0f / (float)WMR;
//
//	for (int y = 0; y <= WMR; y++)
//	{
//		for (int x = 0; x <= WMR; x++)
//		{
//			Vertices[WMRP1 * y + x].x = x * WMSDWMR - 1.0f;
//			Vertices[WMRP1 * y + x].y = 0.0f;
//			Vertices[WMRP1 * y + x].z = 1.0f - y * WMSDWMR;
//		}
//	}
//
//	std::vector<GLfloat> quads;
//
//	//quads.push_back(Vertices[1].x);
//	//quads.push_back(Vertices[1].z);
//	//quads.push_back(Vertices[1].y);
//	//quads.push_back(Vertices[2000].x);
//	//quads.push_back(Vertices[2000].z);
//	//quads.push_back(Vertices[2000].y);
//	//quads.push_back(Vertices[15000].x);
//	//quads.push_back(Vertices[15000].z);
//	//quads.push_back(Vertices[15000].y);
//
//
//	for (int y = 0; y < WMR; y++)
//	{
//		int yp1 = y + 1;
//
//		for (int x = 0; x < WMR; x++)
//		{
//			int xp1 = x + 1;
//
//			int a = WMRP1 * y + x;
//			int b = WMRP1 * y + xp1;
//			int c = WMRP1 * yp1 + xp1;
//			int d = WMRP1 * yp1 + x;
//
//			quads.push_back(Vertices[a].x);
//			quads.push_back(Vertices[a].y);
//			quads.push_back(Vertices[a].z);
//			quads.push_back(Vertices[b].x);
//			quads.push_back(Vertices[b].y);
//			quads.push_back(Vertices[b].z);
//			quads.push_back(Vertices[c].x);
//			quads.push_back(Vertices[c].y);
//			quads.push_back(Vertices[c].z);
//
//			quads.push_back(Vertices[d].x);
//			quads.push_back(Vertices[d].y);
//			quads.push_back(Vertices[d].z);
//			quads.push_back(Vertices[a].x);
//			quads.push_back(Vertices[a].y);
//			quads.push_back(Vertices[a].z);
//			quads.push_back(Vertices[c].x);
//			quads.push_back(Vertices[c].y);
//			quads.push_back(Vertices[c].z);
//		}
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, t);
//	glBufferData(GL_ARRAY_BUFFER, quads.size() * 4, quads.data(), GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//
//	glBindVertexArray(0);
//
//	QuadsVerticesCount = quads.size() / 3;
//
//	quads.clear();
//
//	delete[] Vertices;
//}

//void WaterRenderer::initWaterHeightMaps(GLuint gl_max_texture_max_anisotropy_ext)
//{
//	glActiveTexture(GL_TEXTURE0);
//	glGenTextures(2, WaterHeightMaps);
//
//	int totCount = WHMR_W * WHMR_H;
//	vec4 *Heights = new vec4[totCount];
//
//	for (int i = 0; i < totCount; i++)
//	{
//		Heights[i] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
//		//Heights[i] = vec4(0.0f, (float)(i) / totCount, 0.0f, 0.0f);
//	}
//
//	for (int i = 0; i < 2; i++)
//	{
//		glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[i]);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_max_texture_max_anisotropy_ext);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WHMR_W, WHMR_H, 0, GL_RGBA, GL_FLOAT, Heights);
//		glGenerateMipmapEXT(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//
//	delete[] Heights;
//}

void WaterRenderer::initWaterHeightMaps(GLuint gl_max_texture_max_anisotropy_ext)
{
	glActiveTexture(GL_TEXTURE0);
	//glGenTextures(2, WaterHeightMaps);

	//int totCount = WHMR_W * WHMR_H;
	//vec4 *Heights = new vec4[totCount];

	//for (int i = 0; i < totCount; i++)
	//{
	//	Heights[i] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	//	//Heights[i] = vec4(0.0f, (float)(i) / totCount, 0.0f, 0.0f);
	//}

	// get current settings:
	int view[4];
	glGetIntegerv(GL_VIEWPORT, view);
	float col[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, col);

	// new viewport:
	glViewport(0, 0, WHMR_W, WHMR_H);

	for (int i = 0; i < 2; i++)
	{
		WaterHeightMapsTexture[i] = createEmptyTexture2D(WHMR_W, WHMR_H);

		WaterHeightMapsFrameBuffer[i].createAndBind();
		WaterHeightMapsFrameBuffer[i].attachTextureAsColorTarget(0, WaterHeightMapsTexture[i], WHMR_W, WHMR_H);
		WaterHeightMapsFrameBuffer[i].setDrawBuffers();
		WaterHeightMapsFrameBuffer[i].check();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[i]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_max_texture_max_anisotropy_ext);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WHMR_W, WHMR_H, 0, GL_RGBA, GL_FLOAT, Heights);
		//glGenerateMipmapEXT(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	FrameBuffer::bindSystemFrameBuffer();
	glViewport(view[0], view[1], view[2], view[3]);
	glClearColor(col[0], col[1], col[2], col[3]);
}

void WaterRenderer::initWaterNormalMap(GLuint gl_max_texture_max_anisotropy_ext)
{
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &WaterNormalMap);

	int totCount = WNMR_W * WNMR_H;
	vec4 *Normals = new vec4[totCount];

	for (int i = 0; i < totCount; i++)
	{
		Normals[i] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		//Normals[i] = vec4((float)(i) / totCount, 1.0f, 0.0f, 1.0f);
	}

	glBindTexture(GL_TEXTURE_2D, WaterNormalMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_max_texture_max_anisotropy_ext);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WNMR_W, WNMR_H, 0, GL_RGBA, GL_FLOAT, Normals);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] Normals;
}

GLuint WaterRenderer::createEmptyTexture2D(GLuint w, GLuint h)
{
	GLuint texId;
	glGenTextures(1, &texId);

	//if (texId == 0)
	//{
	//	return 0;
	//}

	glBindTexture(GL_TEXTURE_2D, texId);
	//CHECK_OPENGL_ERRORS();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//CHECK_OPENGL_ERRORS();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//CHECK_OPENGL_ERRORS();

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
	//CHECK_OPENGL_ERRORS();

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