//
// Created by Dominik Schäfer on 1/8/26.
//

#include "Snake.h"
#include <deque>
#include <glm/glm.hpp>

Snake::Snake() : body({glm::vec3(0.0f, 0.0f, 0.0f)}), velocity(glm::vec3(1.0f, 0.0f, 0.0f)) {};

void Snake::update() {
	glm::vec3 newHead = body.front() + velocity;

	body.push_front(newHead);
	body.pop_back();
}
