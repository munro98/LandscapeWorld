#pragma once

#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "Texture.hpp"
#include "Loader.hpp"

class WaterAddDropShader : public Shader
{
public:
	WaterAddDropShader(std::string name);
	~WaterAddDropShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadDropRadius(float);
	void loadDropPosition(glm::vec2 &p);

private:
	GLuint m_location_dropRadius;
	GLuint m_location_dropPosition;
};

