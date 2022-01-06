#include "pch.h"
#include "System.h"
#include "Entity.h"
#include "CoordinateSystem.h"
#include "World.h"


void World::init()
{
	for (auto& system : systems)
	{
		system->init();
	}
}

Entity* World::addEntity(std::string name)
{
	auto ent = std::make_unique<Entity>(name);
	ent->id = next_entity_id;
	ent->world = this;
	entities.insert({ next_entity_id, std::move(ent) });

	return entities.at(next_entity_id++).get();
}

void World::removeEntity(Entity* entity)
{
	for (auto& c : entity->components)
	{
		removeComponent(entity, c.first);
	}

	auto _id = entity->id;

	entities.erase(_id);
}

void World::addComponent(Entity* entity, unsigned int id)
{
	ComponentMask oldMask = entity->signature;

	entity->signature.addComponentByID(id);

	for (auto& system : systems)
	{
		if (entity->signature.isNewMatch(oldMask, system->signature))
		{
			system->registerEntity(entity);
		}
	}
}

void World::removeComponent(Entity* entity, unsigned int id)
{
	ComponentMask oldMask = entity->signature;

	entity->signature.removeComponentByID(id);

	for (auto& system : systems)
	{
		if ( entity->signature.noLongerMatches(oldMask, system->signature) )
		{
			system->unregisterEntity(entity);
		}
	}
}

void World::addSystem(System* system)
{
	system->registerSystem(this);

	auto sysPtr = std::unique_ptr<System>(system);

	systems.push_back(std::move(sysPtr));
}

void World::update(const float dt)
{
	for (auto& sys : systems)
	{
		sys->update(dt);
	}
}

void World::render(sf::RenderTarget* target)
{
	for (auto& sys : systems)
	{
		sys->render(target);
	}
}





