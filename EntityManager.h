#pragma once
#include "EntityFactory.h"

class World;

class EntityManager
{
public:
	void init(World* world);
	Entity* addEntity(std::unique_ptr<Entity> entity);
	void removeEntity(unsigned int entity_id);
	Entity* getEntity(unsigned int entity_id);

private:
	World* _world;
	unsigned int _next_id = 0;
	std::unordered_map<unsigned int, std::unique_ptr<Entity>> _entities;
	EntityFactory _entity_factory;
};

