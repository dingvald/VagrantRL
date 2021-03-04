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

		EntityHandle corpse = parentHub->createEntity(deadEntity.getName() + " corpse");
		if (deadEntity.has<Position>())
		{
			ComponentHandle<Position> position;
			parentHub->unpack(entity, position);

			// Change the layer of the corpse, but keep the same position
			corpse.addComponent(Position(position->x, position->y, gl::ITEMS));

			// Remove the dead object from the map
			parentHub->removeEntityFromMap(entity);
		}

		if (deadEntity.has<Sprite>())
		{
			ComponentHandle<Sprite> sprite;
			parentHub->unpack(entity, sprite);
			sprite->baseColor = sf::Color::Red;
			sprite->spriteNum = 6;

			corpse.addComponent(Sprite(sprite->spriteNum, sprite->baseColor));
		}

		parentHub->destroyEntity(entity);
		parentHub->addEntityToMap(corpse);
	}
};


}// namespace ecs