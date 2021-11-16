#include "pch.h"
#include "Event.h"
#include "Entity.h"
#include "Component.h"



// Base Component Methods

void Component::remove()
{
	
}

void Component::setOwnerTo(Entity* entity)
{
	owner = entity;
}


//////////////////////////////////////////////////////////////////////////////////////////

// Test Component Methods

TestComponent::TestComponent(std::string message)
	: message(message)
{
}

void TestComponent::init()
{
	owner->eventBus.subscribe(this, EventType::testEvent, &TestComponent::onTestEvent);
}

void TestComponent::onTestEvent(Event& ev)
{
	ev.parameters.at("Message") += (message + "\n");
}




// Position Component Methods

PositionComponent::PositionComponent(int x, int y)
	: x(x), y(y)
{
	
}

void PositionComponent::init()
{
	owner->eventBus.subscribe(this, EventType::testEvent, &PositionComponent::onTestEvent);
}

void PositionComponent::onTestEvent(Event& ev)
{
	ev.parameters.at("Message") += ("x: " + std::to_string(x) + ", y: " + std::to_string(y) + "\n");
}



//

