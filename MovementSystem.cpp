#include "pch.h"
#include "MovementSystem.h"

void MovementSystem::init()
{
	signature.addComponentByType<PositionComponent>();

	eventBus->subscribe(this, &MovementSystem::onMoveEvent);
	eventBus->subscribe(this, &MovementSystem::onSwapPlacesEvent);
}

void MovementSystem::move(Entity* entity, sf::Vector2i dir)
{
	auto pos_comp = entity->getComponent<PositionComponent>();
	auto pos = pos_comp->position;
	unsigned int layer = static_cast<unsigned int>(pos_comp->layer);
	
	auto intended_pos = pos + dir;

	if (intended_pos.x < 0 || intended_pos.y < 0 || intended_pos.x > (world->currentMap->getWidth() - 1) || intended_pos.y > (world->currentMap->getHeight() - 1))
	{
		return;
	}

	Entity* entity_in_the_way = checkForCollisionAt(intended_pos);

	if (entity_in_the_way)
	{
		if (entity != entity_in_the_way)
		{
			eventBus->publish(new CollisionEvent(entity, entity_in_the_way));
		}
		else
		{
			eventBus->publish(new SpendTimeEvent(100));
		}
	} 
	else
	{
		world->currentMap->removeEntity(entity, layer, pos);
		world->currentMap->placeEntity(entity, layer, intended_pos);
		pos_comp->position = intended_pos;
		eventBus->publish(new SpendTimeEvent(100));
	}
}

Entity* MovementSystem::checkForCollisionAt(sf::Vector2i coordinate)
{
	if (world->currentMap->getEntitiesAt(static_cast<unsigned int>(gl::Layer::Actor), coordinate))
	{
		return world->currentMap->getEntitiesAt(static_cast<unsigned int>(gl::Layer::Actor), coordinate)->back();
	}
	else
	{
		return nullptr;
	}
}

void MovementSystem::onMoveEvent(MoveEvent* ev)
{
	move(ev->entity, ev->dir);
}

void MovementSystem::onSwapPlacesEvent(SwapPlacesEvent* ev)
{
	auto pos1 = ev->e1->getComponent<PositionComponent>()->position;
	auto pos2 = ev->e2->getComponent<PositionComponent>()->position;

	world->currentMap->removeEntity(ev->e2, static_cast<unsigned int>(gl::Layer::Actor), pos2);
	move(ev->e1, pos2 - pos1);
	world->currentMap->placeEntity(ev->e2, static_cast<unsigned int>(gl::Layer::Actor), pos1);
	
	ev->e2->getComponent<PositionComponent>()->position = pos1;
}
