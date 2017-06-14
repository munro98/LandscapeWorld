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

	void loadwaterNormalMapResolutionWidth(float);
	void loadwaterNormalMapResolutionHeight(float);
	void loadWMSDWNMRM2(float);

private:
	GLuint m_location_model;
	GLuint m_location_view;
	GLuint m_location_projection;
	GLuint m_location_waterNormalMapResolution_W;
	GLuint m_location_waterNormalMapResolution_H;
	GLuint m_location_WMSDWNMRM2;
};

