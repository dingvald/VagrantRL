#pragma once

class Entity;

struct Event
{
};

struct TestEvent : public Event
{
	TestEvent(std::string message) : message(message) {}
	std::string message;
};

struct MoveEvent : public Event
{
	MoveEvent(Entity* entity, sf::Vector2i dir) : entity(entity), dir(dir) {}
	sf::Vector2i dir;
	Entity* entity;
};

struct StartTurnEvent : public Event
{

};

struct SpendTimeEvent : public Event
{
	SpendTimeEvent(unsigned int time) : time(time) {}

	unsigned int time;
};

struct CollisionEvent : public Event
{
	CollisionEvent(Entity* initiator, Entity* receiver) : initiator(initiator), receiver(receiver){}

	Entity* initiator;
	Entity* receiver;
};

struct SwapPlacesEvent : public Event
{
	SwapPlacesEvent(Entity* e1, Entity* e2) : e1(e1), e2(e2) {}
	Entity* e1;
	Entity* e2;
};

struct ViewportMoveEvent : public Event
{
	ViewportMoveEvent(sf::Vector2f origin) : newOrigin(origin) {}

	sf::Vector2f newOrigin;
};

