#pragma once
#include "EventBus.h"
#include "Component.h"
#include "Globals.h"
#include "Map.h"

struct Player
{
	unsigned int id;
	Entity* ptr;
};


// Forward declarations
class System;
//

class World
{
public:
	// Data
	EventBus eventBus;
	Map* map = { nullptr };
	Player player{ 0,nullptr };
	sf::Vector2i worldPosition;
	sf::Vector2f viewportOrigin;

	void init();

	// Dealing with entities

	Entity* registerEntity(std::unique_ptr<Entity> entity);

	void removeEntity(Entity* entity);

	void setAsPlayer(Entity* entity);

	// Dealing with Components

	void addComponent(Entity* entity, unsigned int id);
	void removeComponent(Entity* entity, unsigned int id);

	// Dealing with systems

	void addSystem(System* system);

	void update(const float dt);
	void render(sf::RenderTarget* target);

	// Dealing with persistence

	void save_entities(std::list<unsigned int> list_of_ids);
	std::list<Entity*> load_entities(std::list<unsigned int> list_of_ids);

	void save_player();
	void load_player();

	void save_game();
	void load_game();
	

private:
	unsigned int next_entity_id = { 0 };
	std::vector< std::unique_ptr<System> > systems;
	std::map<unsigned int, std::unique_ptr<Entity> > entities;
	
};

