#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <glm/mat4x2.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
	Shader(std::string name);
	~Shader();

	std::string loadShader(const std::string& fileName);
	GLuint getUniformLocation(std::string uniformName);
	void bindAttribute(int attribute, std::string variableName);
	void loadFloat(int location, float value);
	void loadInt(int location, int value);
	void loadVector(int location, glm::vec3 vector);
	void loadVector(int location, glm::vec2 vector);
	void loadBool(int location, bool value);
	void loadMatrix(int location, glm::mat4 matrix);

	void use();
	static void stop();

	GLuint m_program;


};
