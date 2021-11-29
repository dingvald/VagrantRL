#pragma once
#include "Component.h"

class TestComponent : public ComponentID<TestComponent>
{
private:
	void init() override;

public:
	std::string message;
	TestComponent(std::string message);

	// Event handlers
	void onTestEvent(Event& ev);

};

class TimeComponent : public ComponentID<TimeComponent>
{
private:
	void init() override;

public:
	int speed;
	int built_up_speed;
	TimeComponent(int speed);
};

enum class Layer
{
	Tile,
	Item,
	Actor,
	Effect,
	Total
};

class PositionComponent : public ComponentID<PositionComponent>
{
private:
	void init() override;

public:
	// Data
	sf::Vector2u position;
	Layer layer;
	//
	PositionComponent(unsigned int x, unsigned int y, Layer layer);

	// Event Handlers

	void onTestEvent(Event& ev);
};

class RenderComponent : public ComponentID<RenderComponent>
{
private:
	void init() override;

public:
	unsigned int sprite_id;
	sf::Color color;
	int brightness = { 100 };
	RenderComponent(unsigned int sprite_id, sf::Color color);
};

class PhysicsComponent : public ComponentID<PhysicsComponent>
{
private:
	void init() override;

public:
	bool isBlocking;
	PhysicsComponent(bool isBlocking)
};