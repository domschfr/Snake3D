//
// Created by Dominik Schäfer on 1/8/26.
//

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <string>
#include "Snake.h"

class Game {
public:
	Snake snake;
	uint32_t lastTickTime = 0;
	const uint32_t tickRate = 250;

	Game();
	~Game();

	bool init(const std::string& title, int width, int height);
	void run();
	void clean();
private:
	void processInputs();
	void update();
	void render();

	std::string readFile(const char *filePath);

	void setupShaders();
	void setupCube();
	unsigned int compileShader(unsigned int type, const char* source);

	bool isRunning;

	SDL_Window *window;
	SDL_GLContext glContext;

	unsigned int VAO, VBO, shaderProgram;

	int screenWidth, screenHeight;
};

#endif //GAME_H
