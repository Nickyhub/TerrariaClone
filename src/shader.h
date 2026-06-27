#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

class Shader
{
public:
	// the program ID
	unsigned int m_ID;

	// constructor reads and builds the shader
	Shader(const char *vertexPath, const char *fragmentPath);
	~Shader();
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMatrix4f(const std::string &name, glm::mat4x4 value) const;
	void setVec3(const std::string &name, glm::vec3 value) const;
};
