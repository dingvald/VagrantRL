#pragma once
#include "EventBus.h"
#include "Globals.h"
#include "Map.h"
#include "EntityFactory.h"


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
	EntityFactory entityFactory;
	Player player{ 0,nullptr };
	sf::Vector2i worldPosition;
	sf::Vector2f cameraOrigin;

	void init();
	void cleanUp();

	Entity* registerEntity(std::unique_ptr<Entity> entity);
	std::unique_ptr<Entity> unregisterEntity(Entity* entity);
	void setAsPlayer(Entity* entity);

	void addSystem(System* system);

	void update(const float dt);
	void render(sf::RenderTarget* target);

	void save_player();
	void load_player();

private:
	friend Entity;

	unsigned int next_entity_id = { 1 };
	std::vector< std::unique_ptr<System> > systems;
	std::map<unsigned int, std::unique_ptr<Entity> > entities;

	void addComponent(Entity* entity, unsigned int id);
	void removeComponent(Entity* entity, unsigned int id);
	
};

