//
// Created by Dominik Schäfer on 1/8/26.
//

#ifndef SHADER_H
#define SHADER_H

#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use() const;
	void setMat4(const std::string &name, const glm::mat4 &value) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
private:
	static std::string readFile(const char* path);
	static unsigned int compileShader(unsigned int type, const char* source) ;
};

#endif //SHADER_H
