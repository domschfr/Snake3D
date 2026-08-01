//
// Created by Dominik Schäfer on 1/8/26.
//

#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, readFile(vertexPath).c_str());
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, readFile(fragmentPath).c_str());

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() const {
	glUseProgram(ID);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	int location = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}


std::string Shader::readFile(const char* path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << path << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

unsigned int Shader::compileShader(unsigned int type, const char* source) {
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}