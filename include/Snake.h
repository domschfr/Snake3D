//
// Created by Dominik Schäfer on 1/8/26.
//

#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <glm/glm.hpp>

class Snake {
public:
	std::deque<glm::vec3> body;
	glm::vec3 velocity;

	Snake();

	void update();
};

#endif //SNAKE_H
