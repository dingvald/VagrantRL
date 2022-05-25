#include "pch.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/memory.hpp"
#include "World.h"
#include "System.h"
#include "Entity.h"


void World::init()
{
	entityFactory.init();

	for (auto& system : systems)
	{
		system->init();
	}
}

void World::cleanUp()
{
	for (auto& system : systems)
	{
		system->cleanUp();
	}
}

Entity* World::registerEntity(std::unique_ptr<Entity> entity)
{
	if (entity->getID() == 0)
	{
		entity->id = ++next_entity_id;
	}

	unsigned int entity_id = entity->getID();
	
	entity->world = this;

	auto blank_mask = ComponentMask();

	for (auto& system : systems)
	{
		if (entity->signature.isNewMatch(blank_mask, system->signature))
		{
			system->registerEntity(entity.get());
		}
	}

	entities.insert({ entity_id, std::move(entity) });

	return entities.at(entity_id).get();
}

std::unique_ptr<Entity> World::unregisterEntity(Entity* entity)
{
	for (auto& s : systems)
	{
		s->unregisterEntity(entity);
	}

	auto _id = entity->getID();

	auto unregistered_entity = std::move(entities.at(_id));
	entities.erase(_id);

	return std::move(unregistered_entity);
}

void World::setAsPlayer(Entity* entity)
{
	player.id = entity->getID();
	player.ptr = entity;
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
	{
		load_player();
	}
}

void World::save_player()
{
	std::ofstream os("data/playerData.dat");
	cereal::BinaryOutputArchive archive(os);

	auto p = std::move(entities.at(player.id));

	archive(p);

	unregisterEntity(p.get());
	player.ptr = nullptr;
}

void World::load_player()
{
	std::ifstream is("data/playerData.dat");
	cereal::BinaryInputArchive archive(is);
	auto p = std::make_unique<Entity>("Player");

	archive(p);

	auto player_ptr = registerEntity(std::move(p));
	setAsPlayer(player_ptr);
}






