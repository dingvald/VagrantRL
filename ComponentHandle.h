#pragma once
#include "ComponentManager.h"
#include "Entity.h"

namespace ecs {

template <typename ComponentType>
struct ComponentHandle {
	using ExposedComponentType = typename ComponentManager<ComponentType>::LookupType;

	Entity owner;
	ExposedComponentType* component;
	ComponentManager<ComponentType>* manager;

	ComponentHandle() {};
	ComponentHandle(Entity owner, ExposedComponentType* component, ComponentManager<ComponentType>* manager)
	{
		this->owner = owner;
		this->component = component;
		this->manager = manager;
	}

	ExposedComponentType* operator->() const { return component; }

	void destroy()
	{
		manager->destroyComponent(owner);
	}
};

} // namespace ecs