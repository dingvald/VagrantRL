#include "pch.h"
#include "Components.h"
#include "Map.h"
#include "CoordinateSystem.h"

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

	world->currentMap->placeEntity(entity, (unsigned int)comp->layer, comp->position);
}

void CoordinateSystem::removeEntity(Entity* entity)
{
	auto comp = entity->getComponent<PositionComponent>();

	world->currentMap->removeEntity(entity, (unsigned int)comp->layer, comp->position);
}

