#pragma once
#include "EventBus.h"
#include "Component.h"
#include "Globals.h"
#include "Map.h"

// Forward declarations
class System;
class CoordinateSystem;
//

class World
{
public:
	// Data
	EventBus eventBus;
	Map* currentMap = { nullptr };
	Entity* player;

	void init();

	// Dealing with entities

	Entity* addEntity(std::string name);

	void removeEntity(Entity* entity);

	// Dealing with Components

	void addComponent(Entity* entity, unsigned int id);
	void removeComponent(Entity* entity, unsigned int id);

	// Dealing with systems

	void addSystem(System* system);

	void update(const float dt);
	void render(sf::RenderTarget* target);
	

private:
	unsigned int next_entity_id = { 0 };
	std::vector< std::unique_ptr<System> > systems;
	std::map<unsigned int, std::unique_ptr<Entity> > entities;
	
};

