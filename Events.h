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

struct InteractionEvent : public Event
{
	InteractionEvent(ecs::Entity actingEntity, ecs::Entity destinationEntity)
		: actingEntity(actingEntity), destinationEntity(destinationEntity) {};
	
	ecs::Entity actingEntity, destinationEntity;
};

