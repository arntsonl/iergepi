#include "Game.h"

int main(int argc, char * argv[])
{
	Game * newGame = new Game();
	newGame->Init();
	newGame->Run();
	return 0;
}
