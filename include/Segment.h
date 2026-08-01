//
// Created by Dominik Schäfer on 1/8/26.
//

#ifndef SEGMENT_H
#define SEGMENT_H

#pragma once
#include "Shader.h"
#include <glm/glm.hpp>

class Segment {
public:
	Segment();
	~Segment();

	void draw(const Shader &shader, const glm::vec3 &position) const;
private:
	unsigned int VAO, VBO;
	void setupMesh();
};
#endif //SEGMENT_H
