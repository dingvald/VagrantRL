#include "pch.h"
#include "Event.h"
#include "Components.h"

// Test Component

TestComponent::TestComponent(std::string message)
	: message(message)
{
}

void TestComponent::init()
{
	eventBus->subscribe(this, EventType::testEvent, &TestComponent::onTestEvent);
}

void TestComponent::onTestEvent(Event& ev)
{
	ev.parameters.at("Message") += (message + "\n");
}

// Position Component

PositionComponent::PositionComponent(unsigned int x, unsigned int y, Layer layer) : layer(layer)
{
	position.x = x;
	position.y = y;
}

void PositionComponent::init()
{
	eventBus->subscribe(this, EventType::testEvent, &PositionComponent::onTestEvent);
}

void PositionComponent::onTestEvent(Event& ev)
{
	ev.parameters.at("Message") += ("x: " + std::to_string(position.x) + ", y: " + std::to_string(position.y) + "\n");
}

// Actor Component

void TimeComponent::init()
{

}

TimeComponent::TimeComponent(int speed) : speed(speed), built_up_speed(0)
{
}

// Render Component

void RenderComponent::init()
{
}

RenderComponent::RenderComponent(unsigned int sprite_id, sf::Color color) : sprite_id(sprite_id), color(color)
{
}

PhysicsComponent::PhysicsComponent(bool isBlocking) : isBlocking(isBlocking)
{
}
