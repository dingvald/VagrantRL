#pragma once
#include "pch.h"
#include "ECS.h"
#include "Globals.h"

namespace ecs
{

class HealthSystem : public System
{
private:

public:
	HealthSystem()
	{
		signature.addComponent<Health>();
	}

	void init()
	{
		eventBus->subscribe(this, &HealthSystem::onHealthEvent);
	}

	void onHealthEvent(HealthEvent* healthEvent)
	{
		ComponentHandle<Health> health;
		parentHub->unpack(healthEvent->entity, health);

		health->health += healthEvent->changeInHealth;

		if (health->health >= health->maxHealth)
		{
			health->health = health->maxHealth;
		}
		if (health->health <= 0)
		{
			health->health = 0;

			die(healthEvent->entity);

		}
	}

	void die(Entity entity)
	{
		EntityHandle deadEntity(entity, parentHub);

		deadEntity.setName(deadEntity.getName() + " corpse");
		if (deadEntity.has<Player>())
		{
			//deadEntity.removeComponent<Player>();
		}
		if (deadEntity.has<AI>())
		{
			//deadEntity.removeComponent<AI>();
		}
		if (deadEntity.has<Motion>())
		{
			ComponentHandle<Motion> motion;
			parentHub->unpack(entity, motion);
			motion->dx = 0;
			motion->dy = 0;
			motion->speed = 0;
		}
		if (deadEntity.has<Sprite>())
		{
			ComponentHandle<Sprite> sprite;
			parentHub->unpack(entity, sprite);
			sprite->baseColor = sf::Color::Red;
			sprite->spriteNum = 6;
		}
		if (deadEntity.has<Position>())
		{
			ComponentHandle<Position> position;
			parentHub->unpack(entity, position);
			position->layer = gl::ITEMS;
		}
	}
};


}// namespace ecs