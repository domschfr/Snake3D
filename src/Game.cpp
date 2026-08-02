//
// Created by Dominik Schäfer on 1/8/26.
//

#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

Game::Game() : isRunning(false), inMenu(true), window(nullptr), glContext(nullptr), screenWidth(0), screenHeight(0), isGameOver(false), foodPos() {
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

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
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
	glEnable(GL_MULTISAMPLE);
	shader = std::make_unique<Shader>("assets/shaders/shader.vert", "assets/shaders/shader.frag");
	segment = std::make_unique<Segment>();

	spawnFood();

	//ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330 core");

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
		ImGui_ImplSDL2_ProcessEvent(&event);

		if (event.type == SDL_QUIT) {
			isRunning = false;
		}

		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					isRunning = false;
				if (event.key.keysym.sym == SDLK_SPACE && isGameOver) {
					isGameOver = false;
					snake.body = {glm::vec3(0.0f, 0.0f, 0.0f)};
					snake.velocity = glm::vec3(1.0f, 0.0f, 0.0f);
				}
				if (isGameOver)
					continue;
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
}

void Game::update() {
	if (isGameOver) return;

	uint32_t currentTime = SDL_GetTicks();
	if (currentTime - lastTickTime >= tickRate) {
		glm::vec3 nextHead = snake.body.front() + snake.velocity;

		if (static_cast<int>(nextHead.x) > gridBounds || static_cast<int>(nextHead.x) < -gridBounds || static_cast<int>(nextHead.z) > gridBounds || static_cast<int>(nextHead.z) < -gridBounds) {
			isGameOver = true;
			return;
		}

		for (const glm::vec3 &bodySegment : snake.body) {
			if (nextHead.x == bodySegment.x && nextHead.z == bodySegment.z) {
				isGameOver = true;
				return;
			}
		}

		bool ateFood = (nextHead.x == foodPos.x && nextHead.z == foodPos.z);
		snake.update(ateFood);
		if (ateFood) {
			spawnFood();
		}

		lastTickTime = currentTime;
	}
}

void Game::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (inMenu) {
		ImGui::Begin("Main Menu");
		if (ImGui::Button("Start Game")) {
			inMenu = false;
		}
		if (ImGui::Button("Quit")) {
			isRunning = false;
		}
		ImGui::End();
	}
	else {
		shader->use();

		shader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.2f));
		shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
		shader->setVec3("viewPos", glm::vec3(0.0f, 25.0f, 12.0f));

		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 25.0f, 12.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
												static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
												0.1f,
												100.0f);

		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		for (const glm::vec3 &bodySegment: snake.body) {
			segment->draw(*shader, bodySegment);
		}

		segment->draw(*shader, foodPos);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(window);
}

void Game::clean() {
	std::cout << "Cleaning up..." << std::endl;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dist(-gridBounds, gridBounds - 1);

	bool validPos = false;
	while (!validPos) {
		auto x = static_cast<float>(dist(gen));
		auto z = static_cast<float>(dist(gen));
		foodPos = glm::vec3(x, 0.0f, z);

		validPos = std::find_if(snake.body.begin(), snake.body.end(), [&](const glm::vec3 &p) {
			return p.x == foodPos.x && p.z == foodPos.z;
		}) == snake.body.end();
	}
}
