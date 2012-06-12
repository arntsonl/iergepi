#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager()
{
   playerNames.reserve(sizeof(string)*64);
}

void EntityManager::Update(sf::Time elapsed)
{
    //Update Players
    for(vector<string>::iterator iter = playerNames.begin(); iter != playerNames.end(); iter++){
        players[*iter]->Update(elapsed);
    }

    //Update Entities
    for(vector<Entity*>::iterator iter = ents.begin(); iter != ents.end(); iter++){
        (*iter)->Update(elapsed);
    }
}

void EntityManager::Render(Camera* activeCamera)
{

    //Update Players
    for(vector<std::string>::iterator iter = playerNames.begin(); iter != playerNames.end(); iter++){
        players[*iter]->Render(activeCamera);
    }

    //Update Entities
    for(vector<Entity*>::iterator iter = ents.begin(); iter != ents.end(); iter++){
        (*iter)->Render(activeCamera);
    }


}

//Players
void EntityManager::AddPlayer(string name, Entity* player)
{
    playerNames.insert(playerNames.begin(), name);
    players.insert(pair<std::string, Entity*>(name, player));

#ifdef DEBUG
    cout<<"Added Player name: "<<name<<"\n";
    cout<<"Current Players: ";
    for(vector<string>::iterator iter = playerNames.begin(); iter != playerNames.end(); iter++){
        cout<<players[*iter]->getID()<<" ";
    }
    cout<<"\n";
#endif
}

void EntityManager::RemovePlayer(string name)
{
   players.erase(name);
   playerNames.erase(remove(playerNames.begin(), playerNames.end(), name), playerNames.end());
}

Entity* EntityManager::GetPlayer(std::string name)
{
    return players[name];

}

//Local Entities
void EntityManager::AddEntity(Entity* entity)
{
    ents.insert(ents.begin(), entity);
}

void EntityManager::RemoveEntity(Entity* entity)
{
    ents.erase(std::remove(ents.begin(),ents.end(), entity), ents.end());
}

EntityManager::~EntityManager()
{

}
