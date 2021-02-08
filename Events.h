#pragma once
#include "Entity.h"

#pragma region Event Base Class
class Event
{
protected:
	virtual ~Event() {};
};
#pragma endregion

struct TestEvent : public Event
{
	TestEvent(std::string message) : message{ message } {};
	std::string message;
};

struct LogEvent : public Event
{
	LogEvent(std::string message, sf::Color color) : message{ message }, color{ color } {};

	std::string message;
	sf::Color color;
};

struct MovementEvent : public Event
{
	MovementEvent(ecs::Entity entity, int dx, int dy) : entity(entity), dx(dx), dy(dy) {};

	ecs::Entity entity;
	int dx, dy;
};

struct InteractionEvent : public Event
{
	InteractionEvent(ecs::Entity actingEntity, ecs::Entity destinationEntity)
		: actingEntity(actingEntity), destinationEntity(destinationEntity) {};
	
	ecs::Entity actingEntity, destinationEntity;
};

struct AttackEvent : public Event
{
	AttackEvent(ecs::Entity attacker, ecs::Entity defender)
		: attacker(attacker), defender(defender) {};

	ecs::Entity attacker, defender;
};

struct HealthEvent : public Event
{
	HealthEvent(ecs::Entity entity, int changeInHealth)
		: entity(entity), changeInHealth(changeInHealth) {};

	ecs::Entity entity;
	int changeInHealth;
};

struct ActionEvent : public Event
{
	ActionEvent(int cost) : cost(cost) {};

	int cost;
};

struct StartTurnEvent : public Event
{
	StartTurnEvent(ecs::Entity entity) : entity(entity) {};

	ecs::Entity entity;
};

struct PlayerStartTurnEvent : public Event
{
	PlayerStartTurnEvent(ecs::Entity entity) : entity(entity) {};

	ecs::Entity entity;
};

struct BotStartTurnEvent : public Event
{
	BotStartTurnEvent(ecs::Entity entity) : entity(entity) {};

	ecs::Entity entity;
};

struct GetPlayerInputEvent : public Event
{
	GetPlayerInputEvent(ecs::Entity entity) : entity(entity) {}

	ecs::Entity entity;
};

struct GetBotInputEvent : public Event
{
	GetBotInputEvent(ecs::Entity entity) : entity(entity) {}

	ecs::Entity entity;
};