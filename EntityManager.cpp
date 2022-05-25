#include "pch.h"
#include "EntityManager.h"
#include "World.h"
#include "Entity.h"

void EntityManager::init(World* world)
{
	_world = world;
}

Entity* EntityManager::addEntity(std::unique_ptr<Entity> entity)
{


	return nullptr;
}

void EntityManager::removeEntity(unsigned int entity_id)
{
}

Entity* EntityManager::getEntity(unsigned int entity_id)
{
	return nullptr;
}
