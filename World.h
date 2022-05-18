#pragma once
#include "EventBus.h"
#include "Globals.h"
#include "Map.h"
#include "EntityFactory.h"


struct Player
{
	unsigned int id;
	std::shared_ptr<Entity> ptr;
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
	EntityFactory entityFactory;
	Player player{ 0,nullptr };
	sf::Vector2i worldPosition;
	sf::Vector2f cameraOrigin;

	void init();
	void cleanUp();

	Entity* registerEntity(std::unique_ptr<Entity> entity);
	void setAsPlayer(Entity* entity);

	void addSystem(System* system);

	void update(const float dt);
	void render(sf::RenderTarget* target);

	void save_entities(std::list<unsigned int> list_of_ids);
	std::list<Entity*> load_entities(std::list<unsigned int> list_of_ids);

	void save_player();
	void load_player();

	void save_game();
	void load_game();
	

private:
	friend Entity;

	unsigned int next_entity_id = { 0 };
	std::vector< std::unique_ptr<System> > systems;
	std::map<unsigned int, std::unique_ptr<Entity> > entities;

	void addComponent(Entity* entity, unsigned int id);
	void removeComponent(Entity* entity, unsigned int id);

	void removeEntity(Entity* entity);
	
};

