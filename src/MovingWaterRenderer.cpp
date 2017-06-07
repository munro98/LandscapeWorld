#include "MovingWaterRenderer.hpp"
#include "MovingWaterShader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"


//MovingWaterRenderer::MovingWaterRenderer(glm::mat4& projectionMatrix) : m_shader(MovingWaterShader("movingWaterShader"))
MovingWaterRenderer::MovingWaterRenderer(glm::vec3& lightPosition) : m_shader(MovingWaterShader("movingWaterShader"))
{
	GLint gl_max_texture_max_anisotropy_ext = 0;
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &gl_max_texture_max_anisotropy_ext);

	initTriangles();
	initWaterHeightMaps(gl_max_texture_max_anisotropy_ext);
	initWaterNormalMap(gl_max_texture_max_anisotropy_ext);


	m_shader.use();
	m_shader.loadWaterHightMap(0);
	m_shader.loadWaterNormalMap(1);
	m_shader.loadLightPosition(lightPosition);
	m_shader.stop();

	//m_mesh = OBJLoader::loadObjModel("waterplane");
	//m_texture = Loader::loadTexture("waterNormalMap");
}

MovingWaterRenderer::~MovingWaterRenderer()
{
}


void MovingWaterRenderer::render(glm::mat4& view, glm::mat4& model, glm::mat4& projection, glm::vec3& cameraPosition)
{
	//m_shader.use();
	//Update uniforms
	//m_shader.loadModelMatrix(model);
	//m_shader.loadViewMatrix(view);
	//m_shader.loadProjectionMatrix(projection);
	//m_shader.loadCameraPosition(cameraPosition);

	//glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projection[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&(view * model)[0][0]);

	// Set up the view part of the model view matrix
	//glUseProgram(_waterShader);
	//glScalef(5, 5, 5);
	m_shader.use();
	m_shader.loadCameraPosition(cameraPosition);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[0]);
	//glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[WHMID]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, WaterNormalMap);

	glBindBuffer(GL_ARRAY_BUFFER, WaterVBO);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, QuadsVerticesCount);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_shader.stop();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MovingWaterRenderer::initTriangles()
{
	glGenBuffers(1, &WaterVBO);

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

	std::vector<glm::vec3> quads;

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

			quads.push_back(Vertices[a]);
			quads.push_back(Vertices[b]);
			quads.push_back(Vertices[c]);

			quads.push_back(Vertices[d]);
			quads.push_back(Vertices[a]);
			quads.push_back(Vertices[c]);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, WaterVBO);
	glBufferData(GL_ARRAY_BUFFER, quads.size() * 12, quads.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	QuadsVerticesCount = quads.size();

	quads.clear();

	delete[] Vertices;
}

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