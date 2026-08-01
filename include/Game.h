//
// Created by Dominik Schäfer on 1/8/26.
//

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <string>

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
};

#endif //GAME_H
