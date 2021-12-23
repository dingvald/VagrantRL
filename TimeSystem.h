#pragma once
#include "System.h"


class TimeSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;

private:

	std::deque<Entity*> turn_queue;
	Entity* current_actor;
	Entity* last_actor = nullptr;
	Entity* time_keeper;

	void clearTurnQueue(std::deque<Entity*>& q);
	void refreshTurnQueue();
	void printTurnQueue();
	void distributeSpeed();
	void rotateQueue();

	void onSpendTimeEvent(SpendTimeEvent* ev);
};

