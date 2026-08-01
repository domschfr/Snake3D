#include "../include/Game.h"

int main(int argc, char *argv[]) {
	Game game;

	if (game.init("Snake3D", 800, 600)) {
		game.run();
	}

	game.clean();
	return 0;
}