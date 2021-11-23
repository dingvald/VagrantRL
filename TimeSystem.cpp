#include "pch.h"
#include "Event.h"
#include "Entity.h"
#include "TimeSystem.h"

void TimeSystem::init()
{
	signature.addComponentByType<TimeComponent>();

	//Time keeper defines the game's unit of time.
	time_keeper = world->addEntity("Time Keeper");
	time_keeper->addComponent(new TimeComponent(100));
	time_keeper->getComponent<TimeComponent>()->built_up_speed = 100;
}

void TimeSystem::update(const float dt)
{
	if (registeredEntities.size() != turn_queue.size())
	{
		refreshTurnQueue();
	}

	current_actor = turn_queue.front();
	
	if (current_actor == time_keeper)
	{
		distributeSpeed();

		time_keeper->getComponent<TimeComponent>()->built_up_speed = 0;
		
		refreshTurnQueue();
		current_actor = turn_queue.front();
	}

	auto c = current_actor->getComponent<TimeComponent>();

	if (current_actor != last_actor)
	{
		last_actor = current_actor;
		// fire start turn event
		c->built_up_speed -= 100;
	}
	else
	{
		// fire continue turn event
		c->built_up_speed -= 100;
	}

	if (current_actor->getComponent<TimeComponent>()->built_up_speed <= 0)
	{
		turn_queue.pop();
		turn_queue.push(current_actor);
		last_actor = current_actor;
		current_actor = turn_queue.front();
	}
}

void TimeSystem::clearTurnQueue(std::queue<Entity*>& q)
{
	std::queue<Entity*> empty_q;
	std::swap(q, empty_q);
}

void TimeSystem::refreshTurnQueue()
{
	clearTurnQueue(turn_queue);

	registeredEntities.sort([](Entity*& a, Entity*& b) -> bool
		{
			return (a->getComponent<TimeComponent>()->built_up_speed > b->getComponent<TimeComponent>()->built_up_speed);
		});

	for (auto e : registeredEntities)
	{
		turn_queue.push(e);
	}
}

void TimeSystem::printTurnQueue()
{
	std::cout << "\n";
	std::queue<Entity*> temp_q = turn_queue;
	int count = 1;
	while (!temp_q.empty())
	{
		auto e = temp_q.front();
		auto c = e->getComponent<TimeComponent>();

		std::cout << count << ". " << e->getName() << " Speed: " << c->speed << " Build-up: " << c->built_up_speed << "\n";
		temp_q.pop();
		++count;
	}
	std::cout << "\n";
}

void TimeSystem::distributeSpeed()
{
	for (auto e : registeredEntities)
	{
		auto c = e->getComponent<TimeComponent>();
		c->built_up_speed += c->speed;
	}
}

