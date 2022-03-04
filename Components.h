#pragma once
#include "Globals.h"
#include "EventBus.h"
#include "Component.h"
#include "Entity.h"
#include "SFMLSerialization.h"



class TestComponent : public ComponentID<TestComponent>
{
private:
	void init() override;

public:

	TestComponent() {}

	// Event handlers
	void onTestEvent(TestEvent* ev);

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id);
	}
};

class TimeComponent : public ComponentID<TimeComponent>
{
private:
	void init() override;

public:
	int speed;
	int built_up_speed;
	TimeComponent(int speed = 100);

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id, speed, built_up_speed);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id, speed, built_up_speed);
	}
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
	PositionComponent(sf::Vector2i position = {0,0}, gl::Layer layer = gl::Layer::Tile);

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id, position, layer);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id, position, layer);
	}

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

	RenderComponent(unsigned int sprite_id = 0, sf::Color color = sf::Color::White);

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id, sprite_id, color);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id, sprite_id, color);
	}
};

class HealthComponent : public ComponentID<HealthComponent>
{
private:
	void init() override;

public:
	HealthComponent(int health = 5);
	int health;
	int max_health;

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id, health, max_health);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id, health, max_health);
	}
};

class PhysicsComponent : public ComponentID<PhysicsComponent>
{
private:
	void init() override;

public:
	bool isBlocking;
	PhysicsComponent(bool isBlocking = false);

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id, isBlocking);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id, isBlocking);
	}
};

class MyTurnComponent : public ComponentID<MyTurnComponent>
{
public:
	MyTurnComponent() {}

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id);
	}
};

class PlayerAIComponent : public ComponentID<PlayerAIComponent>
{
public:
	PlayerAIComponent() {}

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id);
	}
};

class AIComponent : public ComponentID<AIComponent>
{
public:
	AIComponent() {}

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id);
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
private:
	void init() override;

public:
	FactionComponent(Faction faction = Faction::neutral);

	Faction faction;

	template<class Archive>
	void save(Archive& ar) const
	{
		int fac = (int)faction;
		ar(owner->id, fac);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id, (int)faction);
	}
};

class ViewportFocusComponent : public ComponentID<ViewportFocusComponent>
{
public:
	ViewportFocusComponent() {}

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id);
	}
};

class OnScreenComponent : public ComponentID<OnScreenComponent>
{
public:
	OnScreenComponent() {}

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(owner->id);
	}
	template<class Archive>
	void load(Archive& ar)
	{
		ar(owner->id);
	}
};


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