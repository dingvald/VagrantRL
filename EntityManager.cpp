#include "pch.h"
#include "EntityManager.h"
#include "World.h"
#include "Entity.h"

void EntityManager::init(World* world)
{
	_world = world;
}

Entity* EntityManager::createEntity(std::string type)
{
	auto entity = _entity_factory.build(type);
	entity->world = this->_world;


	return nullptr;
}

void EntityManager::removeEntity(unsigned int entity_id)
{
}

Entity* EntityManager::getEntity(unsigned int entity_id)
{
	return nullptr;
}
