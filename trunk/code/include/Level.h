#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include "Global.h"
#include "Camera.h"

using namespace std;

class Level{
public:
    Level(std::string levName, std::string spritePath);

    void Update(sf::Time deltaTime);
    void Render(Camera* activeCamera);

    ~Level();

private:
    std::string levelName;

    enum sectionType
    {
        floor,
        wallh1,
        wallh2,
        wallh3,
        ceiling
    };


    struct section
    {
        std::vector<sf::Vector2f> nodeList;
        sectionType type;
    };

    section floorSection;

    std::vector<section> ceilingSections;
    std::vector<section> wall1Sections;
    std::vector<section> wall2Sections;
    std::vector<section> wall3Sections;

    sf::Texture texture;

    sf::RenderTexture floorTexture;
    sf::RenderTexture ceilingTexture;
    sf::RenderTexture wallTexture;

    sf::RenderTexture skyTop;
    sf::RenderTexture skyBot;
    sf::RenderTexture skyRig;
    sf::RenderTexture skyLef;
    sf::RenderTexture skyFro;
    sf::RenderTexture skyBac;

    sf::Sprite floorSprite;
    sf::Sprite wallSprite;
    sf::Sprite ceilingSprite;
    sf::Sprite skyboxSprite;

     sf::Vector2f bounds;

    float scale;
};
