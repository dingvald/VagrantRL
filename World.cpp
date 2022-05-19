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
	entity->id = next_entity_id;
	entity->world = this;
	auto ent = entity.get();
	entities.insert({ next_entity_id, std::move(entity) });

	auto blank_mask = ComponentMask();

	for (auto& system : systems)
	{
		if (ent->signature.isNewMatch(blank_mask, system->signature))
		{
			system->registerEntity(ent);
		}
	}

	return entities.at(next_entity_id++).get();
}

void World::removeEntity(Entity* entity)
{
	for (auto& c : entity->components)
	{
		removeComponent(entity, c.first);
	}

	auto _id = entity->getID();

	entities.erase(_id);
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

void World::save_entities(std::list<unsigned int> list_of_ids)
{
}

std::list<Entity*> World::load_entities(std::list<unsigned int> list_of_ids)
{
	return std::list<Entity*>();
}

void World::save_player()
{
	std::ofstream os("data/playerData.dat");
	cereal::BinaryOutputArchive archive(os);

	auto p = std::move(entities.at(player.id));

	archive(p);

	removeEntity(p.get());
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

void World::save_game()
{
	
}

void World::load_game()
{
}





