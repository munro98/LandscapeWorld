#pragma once

#include "Shader.hpp"
#include "WaterShader.hpp"

class WaterNormalShader : public Shader
{
public:
	WaterNormalShader(std::string name);
	~WaterNormalShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadwaterNormMapDistWidth(float);
	void loadwaterNormMapDistHeight(float);
	void loadDefaultHeight(float);

private:
	GLuint m_location_waterNormalMapDistance_W;
	GLuint m_location_waterNormalMapDistance_H;
	GLuint m_location_defaultHeight;
};

