#pragma once

#include "ComponentHandle.h"
#include "Entity.h"
#include "Hub.h"

namespace ecs {

struct EntityHandle
{
	Entity entity;
	Hub* hub;

	std::string getName();

	void setName(std::string name)
	{
		
	}

	void destroy()
	{
		hub->destroyEntity(entity);
	}

	template <typename ComponentType>
	void addComponent(ComponentType&& component)
	{
		hub->addComponent<ComponentType>(entity, std::forward<ComponentType>(component));
	}

	template <typename ComponentType>
	void removeComponent()
	{
		hub->removeComponent<ComponentType>(entity);
	}

	template <typename ComponentType>

	ComponentHandle<ComponentType> getComponent()
	{
		ComponentHandle<ComponentType> handle;
		hub->unpack(entity, handle);

		return handle;
	}
};

} // namespace ecs