#pragma once
#include "Entity.h"

namespace ecs {

class EntityManager
{
private:
	int lastEntity = 0;
public:
	Entity createEntity();
	void destroy(Entity entity);
};

} // namespace ecs

