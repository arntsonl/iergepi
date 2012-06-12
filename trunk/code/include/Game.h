#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "State.h"
#include "IntroState.h"
//#include "MenuState.h"
#include "GameState.h"
#include "Global.h"
#include "Player.h"

class State;
class IntroState;
//class MenuState;
class GameState;
class Player;

class Game
{
public:
	Game();
	~Game();
	void Init(); // init everything needed for our game.
	void Run();
private:
	void Update();
	void GetInput(uint *); // get the keys
	void Render();
	void Tick();
	void ShutDown();
	bool m_quit;
    State * m_state;
    std::vector<State*> m_statePool; // all our states

	sf::RenderWindow * window;
	sf::Clock clock;
	sf::Time m_elapsedTime; // how much time has actually elapsed..
	uint keysDown; // all keys currently down
	uint mouseDown; // all mouse currently down
};
