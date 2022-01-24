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

void World::setAsPlayer(Entity* entity)
{
	player.id = entity->id;
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
}

sf::Vector2i World::worldToGridPosition(sf::Vector2i world_position)
{
	sf::Vector2i grid_position;
	sf::Vector2i world_map_position;
	sf::Vector2i world_pos;
	world_pos.x = (int)worldPosition.x;
	world_pos.y = (int)worldPosition.y;

	world_pos.x -= 1;
	world_pos.y -= 1;

	if (world_pos.x < 0) world_pos.x = 0;
	if (world_pos.y < 0) world_pos.y = 0;

	grid_position = (world_position / gl::TILE_SIZE) - (world_pos*64);

	return grid_position;
}

void World::save_entities(std::list<unsigned int> list_of_ids)
{
	std::ofstream os("data\\object_data.json");
	cereal::JSONOutputArchive oarchive(os);
	for (auto id : list_of_ids)
	{
		entities.at(id)->save(oarchive);
		removeEntity(entities.at(id).get());
	}
}

std::list<Entity*> World::load_entities(std::list<unsigned int> list_of_ids)
{
	return std::list<Entity*>();
}

void World::save_player()
{
	sf::Clock timer;
	std::cout << "Saving player data..." << "\n";
	timer.restart();
	std::ofstream os("data\\player_data.json");
	cereal::JSONOutputArchive oarchive(os);

	entities.at(player.id)->save(oarchive);
	auto t = timer.getElapsedTime().asSeconds();
	std::cout << "Save Complete! (Time: " << t << ")" << "\n";
}

void World::load_player()
{
}

void World::save_game()
{
	sf::Clock timer;
	
	std::cout << "Saving..." << "\n";
	timer.restart();
	std::ofstream os("data\\data.json");

	cereal::JSONOutputArchive oarchive(os);

	for (auto& e : entities)
	{
		e.second->save(oarchive);
	}
	auto t = timer.getElapsedTime().asSeconds();

	std::cout << "Save Complete! (Time: " << t << ")" << "\n";
}

void World::load_game()
{
}





