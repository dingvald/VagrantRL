#pragma once
#include "System.h"

class Event;

class TimeSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;

private:

	std::queue<Entity*> turn_queue;
	Entity* current_actor;
	Entity* last_actor;
	Entity* time_keeper;

	void clearTurnQueue(std::queue<Entity*>& q);
	void refreshTurnQueue();
	void printTurnQueue();
	void distributeSpeed();
};

