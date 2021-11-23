#include "pch.h"
#include "Entity.h"
#include "Event.h"
#include "World.h"
#include "System.h"

void System::init()
{
}

void System::update(const float dt)
{
}

void System::render(sf::RenderTarget* target)
{
}

void System::registerSystem(World* newWorld)
{
	world = newWorld;
	eventBus = &(world->eventBus);
}

void System::registerEntity(Entity* entity)
{
	registeredEntities.push_back(entity);
}

void System::unregisterEntity(Entity* entity)
{
	registeredEntities.remove(entity);
}
