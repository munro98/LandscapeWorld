#include "MovingWaterRenderer.hpp"
#include "MovingWaterShader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"
#include <glm/gtc/matrix_transform.inl>


//MovingWaterRenderer::MovingWaterRenderer(glm::mat4& projectionMatrix) : m_shader(MovingWaterShader("movingWaterShader"))
MovingWaterRenderer::MovingWaterRenderer(glm::vec3& lightPosition) : m_shader(MovingWaterShader("movingWaterShader"))
//MovingWaterRenderer::MovingWaterRenderer(glm::vec3& lightPosition) : m_shader(Shader("movingWaterShader"))
{
	//GLint gl_max_texture_max_anisotropy_ext = 0;
	//glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &gl_max_texture_max_anisotropy_ext);

	initTriangles();
	//initWaterHeightMaps(gl_max_texture_max_anisotropy_ext);
	//initWaterNormalMap(gl_max_texture_max_anisotropy_ext);


	m_shader.use();
	//m_shader.loadWaterHightMap(0);
	//m_shader.loadWaterNormalMap(1);
	//m_shader.loadLightPosition(lightPosition);
	m_shader.stop();

	//m_mesh = OBJLoader::loadObjModel("waterplane");
	//m_texture = Loader::loadTexture("waterNormalMap");
}

MovingWaterRenderer::~MovingWaterRenderer()
{
}

void MovingWaterRenderer::render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition)
{
	glm::mat4 waterModel = glm::mat4();
	waterModel = glm::rotate(waterModel, -45.0f, glm::vec3(1, 0, 0));
	waterModel = glm::translate(waterModel, glm::vec3(30, 20, 30));
	


	m_shader.use();
	m_shader.loadProjectionMatrix(projection);
	m_shader.loadModelMatrix(waterModel);
	m_shader.loadViewMatrix(view);
	//glm::mat4 m = glm::mat4();
	//glm::mat4 m2 = glm::mat4();
	//glm::mat4 m3 = glm::mat4();
	//m3 = glm::ortho(-1, 1, 1, -1, -1, 100);
	//m = glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	////m = glm::translate(m, glm::vec3(0, 0, -10));
	//m2 = glm::rotate(m2, -45.0f, glm::vec3(1, 0, 0));
	////m = projection * m;	//m_shader.loadProjectionMatrix(m3);
	//m_shader.loadModelMatrix(m2);
	//m_shader.loadViewMatrix(m);
	glBindVertexArray(WaterVBO);
	glDrawArrays(GL_TRIANGLES, 0, QuadsVerticesCount);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	m_shader.stop();
}

//void MovingWaterRenderer::render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition)
//{
//	//m_shader.use();
//	//Update uniforms
//	//m_shader.loadModelMatrix(model);
//	//m_shader.loadViewMatrix(view);
//	//m_shader.loadProjectionMatrix(projection);
//	//m_shader.loadCameraPosition(cameraPosition);
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
//	m_shader.use();
//	//m_shader.loadCameraPosition(cameraPosition);
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
//	m_shader.stop();
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}

void MovingWaterRenderer::initTriangles()
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
	glm::vec3 *Vertices = new glm::vec3[WMRP1 * WMRP1];
	
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
	//glBufferData(GL_ARRAY_BUFFER, t2, vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, points.size() * 4, points.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, quads.size() * 4, quads.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	QuadsVerticesCount = quads.size() / 3;
	
	quads.clear();
	
	delete[] Vertices;
}

//void MovingWaterRenderer::initTriangles()
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

void MovingWaterRenderer::initWaterHeightMaps(GLuint gl_max_texture_max_anisotropy_ext)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(2, WaterHeightMaps);

	int totCount = WHMR_W * WHMR_H;
	glm::vec4 *Heights = new glm::vec4[totCount];

	for (int i = 0; i < totCount; i++)
	{
		//Heights[i] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		Heights[i] = glm::vec4(0.0f, (float)(i) / totCount, 0.0f, 0.0f);
	}

	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_max_texture_max_anisotropy_ext);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WHMR_W, WHMR_H, 0, GL_RGBA, GL_FLOAT, Heights);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	delete[] Heights;
}

void MovingWaterRenderer::initWaterNormalMap(GLuint gl_max_texture_max_anisotropy_ext)
{
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &WaterNormalMap);

	int totCount = WNMR_W * WNMR_H;
	glm::vec4 *Normals = new glm::vec4[totCount];

	for (int i = 0; i < totCount; i++)
	{
		//Normals[i] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		Normals[i] = glm::vec4((float)(i) / totCount, 1.0f, 0.0f, 1.0f);
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