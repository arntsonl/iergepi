#pragma once

#include "State.h"
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <iostream>
#include "Utilities.h"
#include "EntityManager.h"
#include "Player.h"
#include "Level.h"


class GameState : public State
{
public:
    GameState(std::string playerNam, Networker* ne, Player* clientPlaye);
    ~GameState();

    // All corresponding states can use these
    void Input(uint press, uint held, uint mpress, uint mheld, sf::Vector2i mpos, sf::Vector2i mdiff);
    uint Update(sf::Time elapsed);
    void Render(sf::RenderWindow * window);
    void Reset(sf::Vector2i mpos);

private:
    EntityManager* entManager;

    Camera* activeCamera;

    //Passing it up from Game
    std::string playerName;
	Networker* net;
	Player* clientPlayer;
	float mouseSensitivity;
	Level* level;
};
