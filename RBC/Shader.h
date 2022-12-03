#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int id;

	Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
	void activate();

	// Ωsƒ∂shader.glsl
	std::string loadShaderSrc(const char* file);
	GLuint compileShader(const char* file, GLenum type);

	// ∂«ªºUniform≈‹º∆
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
    void setMat4(const std::string& name, glm::mat4 val);
};

#endif