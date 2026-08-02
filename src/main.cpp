#include "../include/Game.h"

int main(int argc, char *argv[]) {
	Game game;

	if (game.init("Snake3D", 1280, 720)) {
		game.run();
	}

	game.clean();
	return 0;
}