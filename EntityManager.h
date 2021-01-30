#pragma once
#include "Entity.h"

namespace ecs {

class EntityManager
{
private:
	int lastEntity = 0;
	std::map<int, std::string> nameMap;
public:
	Entity createEntity(std::string name);
	void destroy(Entity entity);

	void setName(Entity entity, std::string name);
	std::string getName(Entity entity);
};

} // namespace ecs

