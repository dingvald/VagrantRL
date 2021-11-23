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

class PositionComponent : public ComponentID<PositionComponent>
{
private:
	void init() override;

public:
	// Data
	sf::Vector2u position;
	//
	PositionComponent(unsigned int, unsigned int);

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