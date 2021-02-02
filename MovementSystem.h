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
		signature.addComponent<Motion>();
	}

	void init()
	{
	}

	void update(const float dt)
	{
		for (auto& entity : registeredEntities)
		{
			ComponentHandle<Position> position;
			ComponentHandle<Motion> motion;

			parentHub->unpack(entity, position, motion);

			if (motion->dx != 0 || motion->dy != 0)
			{

				int spd = motion->speed;

				// Check if a blocking entity is already on the intended tile,
				Entity obstruction = checkForObstruction(position->x + motion->dx * spd, position->y + motion->dy * spd);

				// if nothing there,
				if (obstruction.id == 0)
				{
					// move,
					position->x += motion->dx * spd;
					position->y += motion->dy * spd;
				}
				// if something is there,
				else
				{
					// stop moving,
					motion->dx = 0;
					motion->dy = 0;

					// and trigger an interaction between the two entities.
					eventBus->publish(new InteractionEvent(entity, obstruction));
				}
			}	
		}
	}

	Entity checkForObstruction(int x, int y);
};

} // namespace ecs