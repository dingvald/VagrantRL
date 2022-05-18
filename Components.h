#pragma once
#include "Globals.h"
#include "Component.h"
#include "Parameters.h"
#include "SpatialConversions.h"


class TimeComponent : public ComponentID<TimeComponent>
{
public:
	int speed;
	int built_up_speed;
	TimeComponent() = default;
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
	
	PositionComponent() = default;
	PositionComponent(sf::Vector2i pos, gl::Layer layer) : position(pos), layer(layer) 
	{
		position = toAbsolutePosition(position);
	}
	PositionComponent(Parameters& params)
	{
		position = toAbsolutePosition({ params.get<int>(), params.get<int>() });
		layer = static_cast<gl::Layer>(params.get<int>());
	}
	Component* clone() override
	{
		return new PositionComponent(position, layer);
	}
};

class MotionComponent : public ComponentID<MotionComponent>
{
public:
	sf::Vector2i direction;

	MotionComponent() = default;
	MotionComponent(sf::Vector2i dir) : direction(dir) {}
	Component* clone() override
	{
		return new MotionComponent(direction);
	}
};

class RenderComponent : public ComponentID<RenderComponent>
{
public:
	unsigned int sprite_id;
	sf::Color color;

	RenderComponent() = default;
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

	HealthComponent() = default;
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

	PhysicsComponent() = default;
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
	MyTurnComponent() = default;
	Component* clone() override
	{
		return new MyTurnComponent();
	}
};

class PlayerAIComponent : public ComponentID<PlayerAIComponent>
{
public:
	PlayerAIComponent() = default;
	PlayerAIComponent(Parameters& params) {}
	Component* clone() override
	{
		return new PlayerAIComponent();
	}
};

class AIComponent : public ComponentID<AIComponent>
{
public:
	AIComponent() = default;
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
	FactionComponent() = default;
	FactionComponent(Faction faction) : faction(faction) {}
	FactionComponent(Parameters& params) {}
	Component* clone() override
	{
		return new FactionComponent(faction);
	}

	Faction faction;
};

class CameraFocusComponent : public ComponentID<CameraFocusComponent>
{
public:
	CameraFocusComponent() = default;
	Component* clone() override
	{
		return new CameraFocusComponent();
	}
};

class OnScreenComponent : public ComponentID<OnScreenComponent>
{
public:
	OnScreenComponent() = default;
	Component* clone() override
	{
		return new OnScreenComponent();
	}
};

#include "cereal/types/polymorphic.hpp"
CEREAL_FORCE_DYNAMIC_INIT(components)