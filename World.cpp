#include "pch.h"
#include "World.h"

Entity* World::addEntity(std::string name)
{
	auto ent = std::make_unique<Entity>(name);
	entities.push_back(std::move(ent));

	return entities.back().get();
}

void World::removeEntity(Entity* entity)
{
	for (auto& e : entities)
	{
		if (e.get() == entity)
		{
			entities.remove(e);
			return;
		}
	}
}
