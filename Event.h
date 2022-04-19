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

struct EntityPlacedEvent : public Event
{
	EntityPlacedEvent(Entity* entity) : entity(entity) {}

	Entity* entity;
};

struct EntityRemovedEvent : public Event
{
	EntityRemovedEvent(Entity* entity) : entity(entity) {}

	Entity* entity;
};

