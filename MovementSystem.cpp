#include "pch.h"
#include "Globals.h"
#include "MovementSystem.h"

namespace ecs
{

Entity MovementSystem::checkForObstruction(int x, int y)
{
	return parentHub->checkForEntityAt(gl::BODIES, x, y);
}

} // namespace ecs