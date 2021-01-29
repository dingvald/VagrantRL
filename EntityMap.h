#pragma once
#include "pch.h"
#include "Entity.h"

namespace ecs {

using ComponentInstance = unsigned int;

struct EntityMap
{
	std::map<Entity, ComponentInstance> entityToInstance;
	std::vector<Entity> instanceToEntity;

	Entity getEntity(ComponentInstance instance) { return instanceToEntity.at(instance); }

	ComponentInstance getInstance(Entity entity) { return entityToInstance.at(entity); }

	void add(Entity entity, ComponentInstance instance)
	{
		entityToInstance.insert({ entity, instance });
		instanceToEntity.resize(instanceToEntity.size() + 1);
		instanceToEntity.at(instance) = entity;
	}

	void update(Entity entity, ComponentInstance instance)
	{
		entityToInstance.at(entity) = instance;
		instanceToEntity.at(instance) = entity;
	}

	void remove(Entity entity)
	{
		entityToInstance.erase(entity);
	}

};

} // namespace ecs