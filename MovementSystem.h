#pragma once
#include "ECS.h"

namespace ecs
{

class MovementSystem : public System
{
private:

public:
	MovementSystem()
	{
		signature.addComponent<Position>();
	}

	void init()
	{
		eventBus->subscribe(this, &MovementSystem::onMovementEvent);
	}

	void onMovementEvent(MovementEvent * dir)
	{
		ComponentHandle<Position> position;

		parentHub->unpack(dir->entity, position);


		// Check if a blocking entity is already on the intended tile,
		Entity obstruction = checkForObstruction(position->x + dir->dx, position->y + dir->dy);

		// if nothing there,
		if (obstruction.id == 0)
		{
			// move,
			position->x += dir->dx;
			position->y += dir->dy;

			eventBus->publish(new ActionEvent(100));
		}
		// if something is there,
		else
		{
			// and trigger an interaction between the two entities.
			eventBus->publish(new InteractionEvent(dir->entity, obstruction));
		}	
	}

	Entity checkForObstruction(int x, int y);
};

} // namespace ecs