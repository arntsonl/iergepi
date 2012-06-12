#pragma once
#include "Entity.h"
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "Utilities.h"

using namespace std;

class EntityManager
{
public:
    EntityManager();

    //Typicals
    void Update(sf::Time elapsed);
    void Render(Camera* activeCamera);

    //Players
    void AddPlayer(std::string name, Entity* player);
    void RemovePlayer(std::string name);
    Entity* GetPlayer(std::string name);


    //Local Entities
    void AddEntity(Entity* entity);
    void RemoveEntity(Entity* entity);

    ~EntityManager();

private:
    std::map <string, Entity*> players;
    std::vector <string> playerNames;
    std::vector <Entity*> ents;
};
