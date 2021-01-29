#include "pch.h"
#include "System.h"

namespace ecs {

void System::registerHub(Hub* hub)
{
	parentHub = hub;
}

void System::registerEntity(Entity const& entity)
{
	registeredEntities.push_back(entity);
}

void System::unRegisterEntity(Entity const& entity)
{
	for (auto it = registeredEntities.begin(); it != registeredEntities.end(); ++it)
	{
		Entity e = *it;
		if (e.id == entity.id)
		{
			registeredEntities.erase(it);
			return;
		}
	}
}

ComponentMask System::getSignature()
{
	return signature;
}

} // namespace ecs