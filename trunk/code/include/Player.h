#pragma once
#include "Entity.h"
#include "Networker.h"
#include "Utilities.h"
#include <cstdio>
#include <vector>

using namespace std;

class Player : public Entity
{
public:
    Player();
    Player(string name, sf::Vector2f initialPosition, string spritePath, bool isPlayer);

    void Update(sf::Time elapsed);
    void Render(Camera* activeCamera);
    void SetVelocity(sf::Vector2f velo){velocity = velo;};
    void AddVelocity(sf::Vector2f velo){velocity = velocity + velo; velocity = Utilities::normalize(velocity, 10.f);};
    void SetAngle(float angle){angleDeg = angle; DIRTY=true;};
    void AddAngle(float angle){angleDeg += angle; DIRTY=true;};
    float GetAngle(){return angleDeg;};

    void SetNetworkedValues(sf::Vector3f pos, float ang){position = pos; angleDeg = ang;};

    Camera* getCamera(){return camera;};

    string getName(){return playerName;};
    bool isAlive(){return alive;};
    bool isDirty(){return DIRTY;};
    void setDirty(bool isit){DIRTY = false;};

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
        GETHIT,
        LOSETAIL,
        GAINTAIL,
        JOIN,
        LEAVE,
        CHAT
    };

private:
    bool isClient;
    bool alive;
    bool DIRTY;

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

    struct animation
    {
        int currentFrame;   //count from 0
        vector<sf::IntRect> frames;
        sf::Time betweenFrames;
    };

    map<string, animation> animations;
    void addAnimation(string, animation);
    void removeAnimation(string);
    std::string currentAnim;
    sf::Clock animationTimer;

};

