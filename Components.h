#pragma once
#include "Globals.h"
#include "Component.h"
#include "Parameters.h"


class TimeComponent : public ComponentID<TimeComponent>
{
public:
	int speed;
	int built_up_speed;
	TimeComponent(int speed) : speed(speed), built_up_speed(0) {}
	TimeComponent(Parameters& params)
	{
		speed = params.get<int>();
		built_up_speed = 0;
	}
	Component* clone() override
	{
		return new TimeComponent(speed);
	}
};

class PositionComponent : public ComponentID<PositionComponent>
{
public:
	sf::Vector2i position;
	gl::Layer layer;
	
	PositionComponent(sf::Vector2i pos, gl::Layer layer) : position(pos), layer(layer) 
	{
		position.x *= gl::TILE_SIZE;
		position.y *= gl::TILE_SIZE;
	}
	PositionComponent(Parameters& params)
	{
		position.x = params.get<int>() * gl::TILE_SIZE;
		position.y = params.get<int>() * gl::TILE_SIZE;
		layer = static_cast<gl::Layer>(params.get<int>());
	}
	Component* clone() override
	{
		return new PositionComponent(position, layer);
	}
};

class RenderComponent : public ComponentID<RenderComponent>
{
public:
	unsigned int sprite_id;
	sf::Color color;

	RenderComponent(unsigned int sprite_id, sf::Color color) : sprite_id(sprite_id), color(color) {}
	RenderComponent(Parameters& params)
	{
		sprite_id = params.get<int>();
		auto s = params.get<std::string>();
		std::stringstream ss;
		unsigned int col;
		ss << std::hex << s;
		ss >> col;

		color = sf::Color(col);
	}
	Component* clone() override
	{
		return new RenderComponent(sprite_id, color);
	}
};

class HealthComponent : public ComponentID<HealthComponent>
{
public:
	int health;
	int max_health;

	HealthComponent(int health) : health(health), max_health(health) {}
	HealthComponent(Parameters& params)
	{
		health = params.get<int>();
		max_health = health;
	}
	Component* clone() override
	{
		return new HealthComponent(health);
	}
};

class PhysicsComponent : public ComponentID<PhysicsComponent>
{
public:
	int weight;
	bool isBlocking;
	PhysicsComponent(int weight, bool isBlocking) : weight(weight), isBlocking(isBlocking) {}
	PhysicsComponent(Parameters& params)
	{
		weight = params.get<int>();
		isBlocking = params.get<bool>();
	}
	Component* clone() override
	{
		return new PhysicsComponent(weight, isBlocking);
	}
};

class MyTurnComponent : public ComponentID<MyTurnComponent>
{
public:
	MyTurnComponent() {}
	Component* clone() override
	{
		return new MyTurnComponent();
	}
};

class PlayerAIComponent : public ComponentID<PlayerAIComponent>
{
public:
	PlayerAIComponent() {}
	PlayerAIComponent(Parameters& params) {}
	Component* clone() override
	{
		return new PlayerAIComponent();
	}
};

class AIComponent : public ComponentID<AIComponent>
{
public:
	AIComponent() {}
	AIComponent(Parameters& params) {}
	Component* clone() override
	{
		return new AIComponent();
	}
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
public:
	FactionComponent(Faction faction) : faction(faction) {}
	FactionComponent(Parameters& params) {}
	Component* clone() override
	{
		return new FactionComponent(faction);
	}

	Faction faction;
};

class ViewportFocusComponent : public ComponentID<ViewportFocusComponent>
{
public:
	ViewportFocusComponent() {}
	Component* clone() override
	{
		return new ViewportFocusComponent();
	}
};

class OnScreenComponent : public ComponentID<OnScreenComponent>
{
public:
	OnScreenComponent() {}
	Component* clone() override
	{
		return new OnScreenComponent();
	}
};

/*
CEREAL_REGISTER_TYPE(TestComponent)
CEREAL_REGISTER_TYPE(TimeComponent)
CEREAL_REGISTER_TYPE(PositionComponent)
CEREAL_REGISTER_TYPE(RenderComponent)
CEREAL_REGISTER_TYPE(HealthComponent)
CEREAL_REGISTER_TYPE(PhysicsComponent)
CEREAL_REGISTER_TYPE(MyTurnComponent)
CEREAL_REGISTER_TYPE(PlayerAIComponent)
CEREAL_REGISTER_TYPE(AIComponent)
CEREAL_REGISTER_TYPE(FactionComponent)
CEREAL_REGISTER_TYPE(ViewportFocusComponent)
CEREAL_REGISTER_TYPE(OnScreenComponent)

CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, TestComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, TimeComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PositionComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RenderComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, HealthComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PhysicsComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MyTurnComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PlayerAIComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, AIComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, FactionComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ViewportFocusComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, OnScreenComponent)
*/