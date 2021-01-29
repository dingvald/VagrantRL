#include "pch.h"
#include "EntityManager.h"

namespace ecs {

Entity EntityManager::createEntity()
{
	lastEntity++;
	return { lastEntity };
}

void EntityManager::destroy(Entity entity)
{
	// empty for now
}

} // namespace ecs
