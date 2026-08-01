//
// Created by Dominik Schäfer on 1/8/26.
//

#include "../include/Game.h"
#include <iostream>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

Game::Game() : isRunning(false), window(nullptr), glContext(nullptr) {}

Game::~Game() {
	clean();
}

bool Game::init(const std::string& title, int width, int height) {
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

	SDL_Window *window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (!window) {
		std::cerr << "Window failed to initialize: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	if (!gl_context) {
		std::cerr << "OpenGL failed to initialize: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetSwapInterval(1);

	std::cout << "Game Engine Initialized Successfully!" << std::endl;
	std::cout << "Vendor:   " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;

	// glGetError();
	// GLuint dummyVAO;
	// glGenVertexArrays(1, &dummyVAO);
	// glBindVertexArray(dummyVAO);

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
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			isRunning = false;
		}
	}
}

void Game::update() {}

void Game::render() {
	// int drawableWidth, drawableHeight;
	// SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
	// glViewport(0, 0, drawableWidth, drawableHeight);
	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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