#include "pch.h"
#include "System.h"
#include "Entity.h"
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
	entities.push_back(std::move(ent));

	entities.back().get()->world = this;

	return entities.back().get();
}

void World::removeEntity(Entity* entity)
{
	for (auto& c : entity->components)
	{
		removeComponent(entity, c.first);
	}

	for (auto& e : entities)
	{
		if (e.get() == entity)
		{
			entities.remove(e);
			return;
		}
	}
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



