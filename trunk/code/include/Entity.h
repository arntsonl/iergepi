#pragma once
//
//    Made by Rob "Mr. Amazing" Lach
//    6*9*12
//

#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Camera.h"

class Entity{
public:
    Entity();

    //Should probably be virtual but keeping it straight atm - RL
    virtual void Update(sf::Time elapsed);
    virtual void Render(Camera* activeCamera);

    int getID(){return entityID;};
    sf::Vector3f getPosition(){ return position;};
    bool isAlive(){return alive;};

    virtual ~Entity();
protected:
    sf::Vector3f position;
    int entityID;
    uint updateID;  //This is packed with each update packet to distinguish newer updates
    bool alive;
};
