//
// Created by Dominik Schäfer on 1/8/26.
//

#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Game::Game() : isRunning(false), window(nullptr), glContext(nullptr), screenWidth(0), screenHeight(0) {
}

Game::~Game() {
	clean();
}

bool Game::init(const std::string &title, int width, int height) {
	screenHeight = height;
	screenWidth = width;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window failed to initialize: " << SDL_GetError() << std::endl;
		return false;
	}

	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		std::cerr << "OpenGL failed to initialize: " << SDL_GetError() << std::endl;
		return false;
	}

	std::cout << "Game Engine Initialized Successfully!" << std::endl;
	std::cout << "Vendor:   " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);
	shader = std::make_unique<Shader>("assets/shaders/shader.vert", "assets/shaders/shader.frag");
	segment = std::make_unique<Segment>();

	spawnFood();

	isRunning = true;
	return true;
}

void Game::run() {
	while (isRunning) {
		processInputs();
		update();
		render();
	}
}

void Game::processInputs() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;
			if ((event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP) && snake.velocity.z == 0)
				snake.velocity = glm::vec3(0.0f, 0.0f, -1.0f);
			if ((event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN) && snake.velocity.z == 0)
				snake.velocity = glm::vec3(0.0f, 0.0f, 1.0f);
			if ((event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT) && snake.velocity.x == 0)
				snake.velocity = glm::vec3(-1.0f, 0.0f, 0.0f);
			if ((event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) && snake.velocity.x == 0)
				snake.velocity = glm::vec3(1.0f, 0.0f, 0.0f);
		}
	}
}

void Game::update() {
	uint32_t currentTime = SDL_GetTicks();
	if (currentTime - lastTickTime >= tickRate) {
		glm::vec3 nextHead = snake.body.front() + snake.velocity;
		bool ateFood = glm::distance(nextHead, foodPos) < 0.1f;
		snake.update(ateFood);
		if (ateFood) {
			spawnFood();
		}

		lastTickTime = currentTime;
	}
}

void Game::render() {
	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use();

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 30.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) screenWidth / (float) screenHeight, 0.1f,
	                                        100.0f);

	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	for (const glm::vec3& bodySegment : snake.body) {
		segment->draw(*shader, bodySegment);
	}

	segment->draw(*shader, foodPos);

	SDL_GL_SwapWindow(window);
}

void Game::clean() {
	std::cout << "Cleaning up..." << std::endl;

	if (glContext) {
		SDL_GL_DeleteContext(glContext);
		glContext = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	SDL_Quit();
}

void Game::spawnFood() {
	bool validPos = false;
	while (!validPos) {
		float x = (float)((rand() % (gridBounds * 2)) - gridBounds);
		float z = (float)((rand() % (gridBounds * 2)) - gridBounds);
		foodPos = glm::vec3(x, 0.0f, z);

		validPos = std::find_if(snake.body.begin(), snake.body.end(), [&](const glm::vec3 &p) {
			return p.x == foodPos.x && p.z == foodPos.z;
		}) == snake.body.end();
	}
}
