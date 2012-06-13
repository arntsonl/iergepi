#pragma once
#include "Entity.h"
#include "Networker.h"
#include "Utilities.h"
#include <cstdio>

using namespace std;

class Player : public Entity
{
public:
    Player();
    Player(string name, sf::Vector2f initialPosition, string spritePath, bool isPlayer);

    void Update(sf::Time elapsed);
    void Render(Camera* activeCamera);
    void SetVelocity(sf::Vector2f velo){velocity = velo;};
    void SetAngle(float angle){angleDeg = angle;};
    void AddAngle(float angle){angleDeg += angle;};

    Camera* getCamera(){return camera;};

    string getName(){return playerName;};

    sf::RenderTexture renderTexture;
    sf::Texture texture;

    ~Player();

    enum VERBS
    {
        SHOOT,
        PICKUP,
        SPAWN,
        DIE,
        KILL,
        LOSETAIL,
        GAINTAIL,
        JOIN,
        LEAVE,
        CHAT
    };

private:
    bool isClient;

    string playerName;
    int score;

    //WEAPONS CODE
    // int bullets;

    Camera* camera;

    sf::Vector2f velocity;
    float frictionCoef;

    float angleDeg;

    //sf::Texture texture;
    sf::Sprite sprite;
    //sf::RenderTexture renderTexture;

    map<string, vector<sf::Vector2i> > animations;
    void addAnimation(string, vector<sf::Vector2i>);
    void removeAnimation(string);


};

