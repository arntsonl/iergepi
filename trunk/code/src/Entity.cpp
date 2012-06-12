#include "Entity.h"

Entity::Entity()
{
updateID = 0;
}

void Entity::Update(sf::Time elapsed)
{
    updateID++;
}

void Entity::Render(Camera* activeCamera)
{

}

Entity::~Entity()
{

}
