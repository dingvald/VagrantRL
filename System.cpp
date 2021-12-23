#include "pch.h"
#include "Entity.h"
#include "EventBus.h"
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
	this->addedEntity(entity);
}

void System::unregisterEntity(Entity* entity)
{
	registeredEntities.remove(entity);
	this->removeEntity(entity);
}

void System::addedEntity(Entity* entity)
{
}

void System::removeEntity(Entity* entity)
{
}
