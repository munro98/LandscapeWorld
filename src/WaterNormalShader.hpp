#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class WaterNormalShader : public Shader
{
public:
	WaterNormalShader(std::string name);
	~WaterNormalShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadModelMatrix(glm::mat4 mat);
	void loadViewMatrix(glm::mat4 mat);
	void loadProjectionMatrix(glm::mat4 mat);

	void loadCameraPosition(glm::vec3 &p);
	void loadLightPosition(glm::vec3 &p);
	void loadWaterHightMap(GLuint);
	void loadWaterNormalMap(GLuint);

private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
	GLuint m_location_cameraPosition;
	GLuint m_location_LightPosition;
	GLuint m_location_WaterHightMap;
	GLuint m_location_WaterNormalMap;
};

