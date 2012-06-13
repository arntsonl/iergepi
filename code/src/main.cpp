#include <time.h>
#include "Game.h"

int main(int argc, char * argv[])
{
    srand((unsigned)time(NULL));
	Game * newGame = new Game();
	newGame->Init();
	newGame->Run();
	return 0;
}
