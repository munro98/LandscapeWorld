#include "shader.hpp"


Shader::Shader(std::string name)
{
	m_program = glCreateProgram();
	std::string output = loadShader("./res/" + name + ".vert.c");
	const char *vertexShaderSource = output.c_str();
	//std::cout << vertexShaderSource;

	std::string output2 = loadShader("./res/" + name + ".frag.c");
	const char *fragmentShaderSource = output2.c_str();
	//std::cout << fragmentShaderSource;

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR.SHADER.VERTEX.COMPILATION FAIL\n" << infoLog << std::endl;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR.SHADER.FRAGMENT.COMPILATION FAIL\n" << infoLog << std::endl;
	}

	glGetShaderiv(m_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR.SHADER.Program.Link FAIL\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


Shader::~Shader()
{
}

GLuint Shader::getUniformLocation(std::string uniformName) {
	return glGetUniformLocation(m_program, uniformName.c_str());
}

void Shader::bindAttribute(int attribute, std::string variableName) {
	glBindAttribLocation(m_program, attribute, variableName.c_str());
}

void Shader::loadFloat(int location, float value) {
	glUniform1f(location, value);
}

void Shader::loadInt(int location, int value) {
	glUniform1i(location, value);
}

void Shader::loadVector(int location, glm::vec3 vector) {
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::loadVector(int location, glm::vec2 vector) {
	glUniform2f(location, vector.x, vector.y);
}

void Shader::loadBool(int location, bool value) {
	float toLoad = 0;
	if (value) {
		toLoad = 1;
	}
	glUniform1f(location, toLoad);
}

void Shader::loadMatrix(int location, glm::mat4 matrix) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::loadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;
	
	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			
			output.append(line + "\n");
		}

	}
	else {
		std::cerr << "Shader load error " << fileName << std::endl;
	}

	file.close();

	return output;
}

void Shader::use() {
	glUseProgram(m_program);
}

void Shader::stop() {
	glUseProgram(0);
}