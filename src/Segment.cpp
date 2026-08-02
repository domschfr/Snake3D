//
// Created by Dominik Schäfer on 1/8/26.
//

#include "Segment.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <glm/gtc/matrix_transform.hpp>

Segment::Segment() : VAO(0), VBO(0) {
	setupMesh();
}

Segment::~Segment() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Segment::setupMesh() {
	float vertices[] = {
		//positions (3) + normals (3)
		//back face
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		//front face
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		//left face
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f, 0.0f,

		//right face
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 0.0f,

		//bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,

		//top face
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Segment::draw(const Shader &shader, const glm::vec3 &position) const {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	shader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
