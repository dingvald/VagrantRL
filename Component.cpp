#include "pch.h"
#include "Event.h"
#include "Entity.h"
#include "Component.h"



// Base Component Methods

void Component::setOwnerTo(Entity* entity)
{
	owner = entity;
	eventBus = &(owner->eventBus);
}


//////////////////////////////////////////////////////////////////////////////////////////

// Test Component Methods

