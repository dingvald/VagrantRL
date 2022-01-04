#include "pch.h"
#include "Globals.h"
#include "Entity.h"
#include "Components.h"

// Test Component

void TestComponent::init()
{
	eventBus->subscribe(this, &TestComponent::onTestEvent);
}

void TestComponent::onTestEvent(TestEvent* ev)
{
	ev->message += (" -> " + owner->getName());

	std::cout << ev->message << "\n";
}

// Position Component

PositionComponent::PositionComponent(unsigned int x, unsigned int y, gl::Layer layer) : layer(layer)
{
	position.x = x*gl::TILE_SIZE;
	position.y = y*gl::TILE_SIZE;
}

void PositionComponent::init()
{
	eventBus->subscribe(this, &PositionComponent::onTestEvent);
}

void PositionComponent::onTestEvent(TestEvent* ev)
{
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

void PhysicsComponent::init()
{
}

PhysicsComponent::PhysicsComponent(bool isBlocking) : isBlocking(isBlocking)
{
}

// Health Component


void HealthComponent::init()
{
}

HealthComponent::HealthComponent(int health) : health(health), max_health(health)
{

}

// Faction Component

void FactionComponent::init()
{
}

FactionComponent::FactionComponent(Faction faction) : faction(faction)
{
}
