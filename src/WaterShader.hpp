#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class WaterShader : public Shader
{
public:
	WaterShader(std::string name);
	WaterShader(std::string name, std::string fragmentName);
	~WaterShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadModelMatrix(glm::mat4 & mat);
	void loadViewMatrix(glm::mat4 & mat);
	void loadProjectionMatrix(glm::mat4 & mat);


	void loadCameraPosition(glm::vec3 &p);
	void loadLightPosition(glm::vec3 &p);
	void loadWaterHeightMap(GLuint);
	void loadWaterNormalMap(GLuint);
	void loadWaterColour(glm::vec4 &p);
//	void loadShowColour(bool showColor);

private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
	GLuint m_location_cameraPosition;
	GLuint m_location_LightPosition;
	GLuint m_location_WaterHeightMap;
	GLuint m_location_WaterNormalMap;
	GLuint m_location_WaterColour;
//	GLuint m_location_ShowColour;
};

