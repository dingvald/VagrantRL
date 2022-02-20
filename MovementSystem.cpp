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
	auto pos = (pos_comp->position);
	unsigned int layer = static_cast<unsigned int>(pos_comp->layer);
	
	auto intended_pos = pos + (dir * gl::TILE_SIZE);

	if (intended_pos.x < 0 || intended_pos.y < 0)
	{
		return;
	}

	Entity* entity_in_the_way = checkForCollisionAt(intended_pos);

	if (entity_in_the_way)
	{
		if (entity != entity_in_the_way)
		{
			eventBus->publish(std::make_unique<CollisionEvent>(entity, entity_in_the_way).get());
		}
		else
		{
			eventBus->publish(std::make_unique<SpendTimeEvent>(100).get());
			std::cout << "You are in the way!\n";
		}
	} 
	else
	{
		// Actually perform the move
		world->map->removeEntity(entity, layer, pos);
		world->map->placeEntity(entity, layer, intended_pos);
		pos_comp->position = intended_pos;
		eventBus->publish(std::make_unique<SpendTimeEvent>(100).get());
	}
}

Entity* MovementSystem::checkForCollisionAt(sf::Vector2i coordinate)
{
	auto list_ptr = world->map->getEntitiesAt(static_cast<unsigned int>(gl::Layer::Actor), coordinate);

	if (list_ptr && !list_ptr->empty())
	{
		return list_ptr->back();
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

	world->map->removeEntity(ev->e2, static_cast<unsigned int>(gl::Layer::Actor), pos2);
	move(ev->e1, pos2 - pos1);
	world->map->placeEntity(ev->e2, static_cast<unsigned int>(gl::Layer::Actor), pos1);
	
	ev->e2->getComponent<PositionComponent>()->position = pos1;
}
