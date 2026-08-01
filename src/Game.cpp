//
// Created by Dominik Schäfer on 1/8/26.
//

#include "../include/Game.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Game::Game() : isRunning(false), window(nullptr), glContext(nullptr) {}

Game::~Game() {
	clean();
}

bool Game::init(const std::string& title, int width, int height) {
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
	setupShaders();
	setupCube();

	isRunning = true;
	return true;
}

std::string Game::readFile(const char *filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}


unsigned int Game::compileShader(unsigned int type, const char *source) {
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

void Game::setupShaders() {
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, readFile("assets/shaders/shader.vert").c_str());
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, readFile("assets/shaders/shader.frag").c_str());

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Game::setupCube() {
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
		snake.update();
		lastTickTime = currentTime;
	}
}

void Game::render() {
	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 15.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

	int modelLoc = glGetUniformLocation(shaderProgram, "model");
	int viewLoc = glGetUniformLocation(shaderProgram, "view");
	int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);

	for (const glm::vec3& segment : snake.body) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), segment);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);

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