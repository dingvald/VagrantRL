#pragma once
#include "ComponentMask.h"
#include "Entity.h"
#include "EventBus.h"

namespace ecs {

class Hub;

class System
{
private:

protected:
	std::vector<Entity> registeredEntities;
	Hub* parentHub;

	ComponentMask signature;

public:
	System() = default;
	virtual ~System() = default;
	System(const System&) = default;
	System& operator=(const System&) = default;
	System(System&&) = default;
	System& operator=(System&&) = default;

	EventBus* eventBus;

	// Called before the game starts but after the world inits
	virtual void init() {};

	// Called every game update
	virtual void update(const float dt) {};

	// Called every frame
	virtual void render(sf::RenderTarget* target) {};

	// When a system is added, the hub will register itself
	void registerHub(Hub* hub);

	// When a component is added such that this system should begin acting on it
	// register will be called
	void registerEntity(Entity const& entity);

	// If a component is removed from an entity such that the system should
	// stop acting on it, unregister will be called
	void unRegisterEntity(Entity const& entity);

	// Returns the signature of the system
	ComponentMask getSignature();

};

} // namespace ecs