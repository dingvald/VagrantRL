#include "pch.h"
#include "CoordinateSystem.h"
#include "Components.h"
#include "Map.h"


void CoordinateSystem::init()
{
	signature.addComponentByType<PositionComponent>();
}

void CoordinateSystem::update(const float dt)
{
}

void CoordinateSystem::addedEntity(Entity* entity)
{
	auto comp = entity->getComponent<PositionComponent>();

	world->map->placeEntity(entity, (unsigned int)comp->layer, comp->position);
}

void CoordinateSystem::removeEntity(Entity* entity)
{
	auto comp = entity->getComponent<PositionComponent>();

	world->map->removeEntity(entity, (unsigned int)comp->layer, comp->position);
}

