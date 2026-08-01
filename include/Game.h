//
// Created by Dominik Schäfer on 1/8/26.
//

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "Snake.h"
#include "Shader.h"
#include "Segment.h"
#include <memory>

class Game {
public:
	Game();
	~Game();

	bool init(const std::string& title, int width, int height);
	void run();
	void clean();
private:
	void processInputs();
	void update();
	void render();

	bool isRunning;
	SDL_Window *window;
	SDL_GLContext glContext;
	int screenWidth, screenHeight;

	uint32_t lastTickTime = 0;
	const uint32_t tickRate = 250;

	Snake snake;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Segment> segment;
};

#endif //GAME_H
