#pragma once

#include <SFML/OpenGL.hpp>
#include <cmath>
#include <iostream>

class Utilities
{
public:
    static float getMagnitude(float x, float y, float z)
    {
        return sqrt(x*x+y*y+z*z);
    }

    static sf::Vector3f normalize(sf::Vector3f vec)
    {
        float length = getMagnitude(vec.x,vec.y,vec.z);
        sf::Vector3f temp;
        temp.x = vec.x/length;
        temp.y = vec.y/length;
        temp.z = vec.z/length;
        return temp;
    }

    static sf::Vector3f cross(sf::Vector3f v1, sf::Vector3f v2)
    {
        sf::Vector3f temp;
        temp.x = v1.y*v2.z - v1.z*v2.y;
        temp.y = v1.z*v2.x - v1.x*v2.z;
        temp.z = v1.x*v2.y - v1.y*v2.x;
        return temp;
    }

    static float dot(sf::Vector3f v1, sf::Vector3f v2)
    {
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    }

    static float billboardAngle(sf::Vector2f v1, sf::Vector2f v2)
    {
        return atan2(v1.x - v2.x,v1.y - -v2.y)*57.2957795;
    }

    static float billboardAngle(sf::Vector3f v1, sf::Vector3f v2)
    {
        return billboardAngle(sf::Vector2f(v1.x,v1.z), sf::Vector2f(v2.x,v2.z));
    }
};
