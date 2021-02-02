#pragma once
#include "pch.h"
#include "Entity.h"
#include "EntityMap.h"

namespace ecs {

template <typename ComponentType>
struct ComponentData
{
	std::vector<ComponentType> data;
};

class BaseComponentManager
{
public:
	BaseComponentManager() = default;
	virtual ~BaseComponentManager() = default;
	BaseComponentManager(const BaseComponentManager&) = default;
	BaseComponentManager& operator=(const BaseComponentManager&) = default;
	BaseComponentManager(BaseComponentManager&&) = default;
	BaseComponentManager& operator=(BaseComponentManager&&) = default;
};

template <typename ComponentType>
class ComponentManager : public BaseComponentManager
{
private:
	ComponentData <ComponentType> componentData;
	EntityMap entityMap;

public:
	using LookupType = ComponentType;

	ComponentManager()
	{
	}

	ComponentInstance addComponent(Entity entity, ComponentType& component)
	{
		componentData.data.push_back(component);
		ComponentInstance newInstance = componentData.data.size()-1;
		entityMap.add(entity, newInstance);

		return newInstance;
	}

	void destroyComponent(Entity entity)
	{
		ComponentInstance instance = entityMap.getInstance(entity);

		ComponentInstance lastComponent = componentData.data.back();
		componentData.data[instance] = componentData.data[lastComponent];
		componentData.data.pop_back();
		Entity lastEntity = entityMap.getEntity(lastComponent);

		//update map
		entityMap.remove(entity);
		entityMap.update(lastEntity, instance);

	}

	LookupType* lookup(Entity entity)
	{
		ComponentInstance instance = entityMap.getInstance(entity);
		return &componentData.data.at(instance);
	}

	bool hasComponent(Entity entity)
	{
		if (entityMap.entityToInstance.count(entity))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

} // namespace ecs
