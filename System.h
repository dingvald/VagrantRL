#pragma once
#include "Entity.h"
#include "ComponentMask.h"
#include "Components.h"


class EventBus;
class Event;
class World;

class System
{
public:
	ComponentMask signature;

	virtual void init();
	virtual void update(const float dt);
	virtual void render(sf::RenderTarget* target);
	virtual void registerSystem(World* newWorld);
	void registerEntity(Entity* entity);
	void unregisterEntity(Entity* entity);

protected:
	EventBus* eventBus;
	World* world;
	std::list<Entity*> registeredEntities;
};

