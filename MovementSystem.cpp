#include "pch.h"
#include "Components.h"
#include "MovementSystem.h"

void MovementSystem::init()
{
	signature.addComponentByType<PositionComponent>();
	signature.addComponentByType<PhysicsComponent>();
}

void MovementSystem::update(const float dt)
{
}

void MovementSystem::addedEntity(Entity* entity)
{
	auto pos = entity->getComponent<PositionComponent>();
	unsigned int layer = static_cast<unsigned int>(pos->layer);
	unsigned int x = pos->position.x;
	unsigned int y = pos->position.y;

	if (map[layer][x][y])
	{
		std::cout << "Attempting to place an entity in an already-occupied space" << ": " << x << ", " << y << "\n";
		return;
	}
	else
	{
		map[layer][x][y] = entity;
	}
}

void MovementSystem::removeEntity(Entity* entity)
{
	auto pos = entity->getComponent<PositionComponent>();
	unsigned int layer = static_cast<unsigned int>(pos->layer);
	unsigned int x = pos->position.x;
	unsigned int y = pos->position.y;

	if (map[layer][x][y])
	{
		map[layer][x][y] = nullptr;
	}
	else
	{
		std::cout << "Attempting to remove entity from unoccupied space!" << ": " << x << ", " << y << "\n";
	}
}
