#include "pch.h"
#include "EntityManager.h"

namespace ecs {

Entity EntityManager::createEntity(std::string name)
{
	lastEntity++;
	nameMap.insert({ lastEntity, name });
	return { lastEntity };
}

void EntityManager::destroy(Entity entity)
{
	nameMap.erase(entity.id);
}

void EntityManager::setName(Entity entity, std::string name)
{
	nameMap.at(entity.id) = name;
}

std::string EntityManager::getName(Entity entity)
{
	return (nameMap.at(entity.id));
}

} // namespace ecs
