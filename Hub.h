#pragma once
#include "Component.h"
#include "ComponentHandle.h"
#include "ComponentManager.h"
#include "ComponentMask.h"
#include "Entity.h"
#include "EntityManager.h"
#include "System.h"
#include "Map.h"

namespace ecs {

struct EntityHandle;

class Hub
{
private:
	std::unique_ptr<EntityManager> entityManager;
	std::vector<std::unique_ptr<System>> systems;
	std::vector<std::unique_ptr<BaseComponentManager>> componentManagers;
	std::map<Entity, ComponentMask> entityMasks;
	std::unique_ptr<Map> map;
	
	void updateEntityMask(Entity const& entity, ComponentMask oldMask);

	template <typename ComponentType>
	ComponentManager<ComponentType>* getComponentManager()
	{
		int family = GetComponentFamily<ComponentType>();

		if (family >= componentManagers.size())
		{
			componentManagers.resize(family + 1);
		}

		if (!componentManagers[family])
		{
			componentManagers[family] = std::make_unique<ComponentManager<ComponentType>>();
		}

		return static_cast<ComponentManager<ComponentType>*>(componentManagers[family].get());
	}

public:
	

	explicit Hub(std::unique_ptr<EntityManager> entityManager);

	// called before first update, after instantiation
	void init(EventBus* eventBus);

	void update(const float dt);

	void render(sf::RenderTarget* target);
	EntityHandle createEntity(std::string name);
	void addSystem(std::unique_ptr<System> system);
	void destroyEntity(Entity entity);
	Entity int2ent(int id)
	{
		return { id };
	}

	// Functions for working with the physical map

	// Returns the map pointer
	Map* getMap()
	{
		return map.get();
	}

	Entity checkForEntityAt(int layer, int x, int y);
	void addEntityToMap(EntityHandle entity);
	void removeEntityFromMap(Entity entity);
	void moveEntityOnMap(Entity entity, int x, int y);

	/*
	Missing a section on "bridge component managers" refer to nomad ecs for implementation
	*/

	template<typename ComponentType>
	void addComponent(Entity const& entity, ComponentType&& component)
	{
		ComponentManager<ComponentType>* manager = getComponentManager<ComponentType>();
		manager->addComponent(entity, component);

		ComponentMask oldMask = entityMasks[entity];
		entityMasks[entity].addComponent<ComponentType>();

		updateEntityMask(entity, oldMask);
	}

	template <typename ComponentType>
	void removeComponent(Entity const& entity) {
		ComponentManager<ComponentType>* manager = getComponentManager<ComponentType>();
		ComponentHandle<ComponentType> component = manager->lookup(entity);
		component.destroy();

		ComponentMask oldMask = entityMasks[entity];
		entityMasks[entity].removeComponent<ComponentType>();

		updateEntityMask(entity, oldMask);
	}

	template <typename ComponentType, typename... Args>
	void unpack(Entity e, ComponentHandle<ComponentType>& handle, ComponentHandle<Args>&... args) {
		typedef ComponentManager<ComponentType> ComponentManagerType;
		auto mgr = getComponentManager<ComponentType>();
		handle = ComponentHandle<ComponentType>(e, mgr->lookup(e), mgr);

		// Recurse
		unpack<Args...>(e, args...);
	}

	// Base case
	template <typename ComponentType>
	void unpack(Entity e, ComponentHandle<ComponentType>& handle) {
		typedef ComponentManager<ComponentType> ComponentManagerType;
		auto mgr = getComponentManager<ComponentType>();
		handle = ComponentHandle<ComponentType>(e, mgr->lookup(e), mgr);
	}

	std::string getEntityName(Entity entity)
	{
		return entityManager->getName(entity);
	}

	void setEntityName(Entity entity, std::string name)
	{
		entityManager->setName(entity, name);
	}

	template <typename ComponentType>
	bool entityHasComponent(Entity entity)
	{
		auto mgr = getComponentManager<ComponentType>();

		return mgr->hasComponent(entity);
	}
};

} // namespace ecs