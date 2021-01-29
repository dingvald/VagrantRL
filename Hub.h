#pragma once
#include "Component.h"
#include "ComponentHandle.h"
#include "ComponentManager.h"
#include "ComponentMask.h"
#include "Entity.h"
#include "EntityManager.h"
#include "System.h"

namespace ecs {

struct EntityHandle;

class Hub
{
private:
	std::unique_ptr<EntityManager> entityManager;
	std::vector<std::unique_ptr<System>> systems;
	std::vector<std::unique_ptr<BaseComponentManager>> componentManagers;
	std::map<Entity, ComponentMask> entityMasks;

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
	EntityHandle createEntity();
	void addSystem(std::unique_ptr<System> system);
	void destroyEntity(Entity entity);

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
};

} // namespace ecs