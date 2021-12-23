#pragma once
#include "Globals.h"
#include "EventBus.h"
#include "Component.h"

class TestComponent : public ComponentID<TestComponent>
{
private:
	void init() override;

public:

	// Event handlers
	void onTestEvent(TestEvent* ev);

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
	sf::Vector2i position;
	gl::Layer layer;
	//
	PositionComponent(unsigned int x, unsigned int y, gl::Layer layer);

	// Event Handlers

	void onTestEvent(TestEvent* ev);
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

class HealthComponent : public ComponentID<HealthComponent>
{
private:
	void init() override;

public:
	HealthComponent(int health);
	int health;
	int max_health;
};

class PhysicsComponent : public ComponentID<PhysicsComponent>
{
private:
	void init() override;

public:
	bool isBlocking;
	PhysicsComponent(bool isBlocking);
};

class MyTurnComponent : public ComponentID<MyTurnComponent>
{
};

class PlayerAIComponent : public ComponentID<PlayerAIComponent>
{

};

class AIComponent : public ComponentID<AIComponent>
{

};

enum class Faction
{
	goodguys,
	badguys,
	neutral,
	total
};

class FactionComponent : public ComponentID<FactionComponent>
{
private:
	void init() override;

public:
	FactionComponent(Faction faction);

	Faction faction;
};

class ViewportFocusComponent : public ComponentID<ViewportFocusComponent>
{

};

class OnScreenComponent : public ComponentID<OnScreenComponent>
{

};
