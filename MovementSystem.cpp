#include "pch.h"
#include "Globals.h"
#include "MovementSystem.h"

namespace ecs
{

Entity MovementSystem::checkForObstruction(int x, int y)
{
	for (auto& entity : registeredEntities)
	{
		ComponentHandle<Position> position;

		parentHub->unpack(entity, position);

		if (position->x == x && position->y == y && position->layer == gl::Layer::BODIES)
		{
			return entity;
		}
	}
	return Entity{ 0 };
}

} // namespace ecs