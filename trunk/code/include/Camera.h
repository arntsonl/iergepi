#pragma once
#include <SFML/Graphics.hpp>
#include "Global.h"

class Camera
{
public:
    Camera(){ position = sf::Vector3f(0,0,0); angleDeg = 0;};
    ~Camera(){};

    sf::Vector3f position;
    float angleDeg;
    sf::RenderWindow* activeWindow;
};
