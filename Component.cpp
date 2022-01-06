#include "pch.h"
#include "EventBus.h"
#include "Entity.h"
#include "Component.h"



// Base Component Methods

void Component::setOwnerTo(Entity* entity)
{
	owner = entity;
	owner_id = entity->id;
	eventBus = &(owner->eventBus);
}


//////////////////////////////////////////////////////////////////////////////////////////

// Test Component Methods

