#include "pch.h"
#include "TimeSystem.h"
#include "Event.h"
#include "Entity.h"
#include "Components.h"


void TimeSystem::init()
{
	signature.addComponentByType<TimeComponent>();

	eventBus->subscribe(this, &TimeSystem::onSpendTimeEvent);

	//Time keeper defines the game's unit of time.
	auto t_keeper = std::make_unique<Entity>("Time Keeper");
	time_keeper = t_keeper.get();
	t_keeper->addComponent(new TimeComponent(100));
	t_keeper->getComponent<TimeComponent>()->built_up_speed = 0;
	current_actor = time_keeper;

	world->registerEntity(std::move(t_keeper));
}

void TimeSystem::update(const float dt)
{
	if (registeredEntities.size() != turn_queue.size())
	{
		refreshTurnQueue();
	}
	
	if (current_actor == time_keeper)
	{
		distributeSpeed();
		time_keeper->getComponent<TimeComponent>()->built_up_speed = 0;
		refreshTurnQueue();
	}

	if (!current_actor->getComponent<MyTurnComponent>())
	{
		current_actor->addComponent(new MyTurnComponent);
	}

	auto time_comp = current_actor->getComponent<TimeComponent>();

	if (current_actor != last_actor)
	{
		last_actor = current_actor;
	}

	if (time_comp->built_up_speed <= 0)
	{
		current_actor->removeComponent<MyTurnComponent>();
		rotateQueue();
	}
} 

void TimeSystem::clearTurnQueue(std::deque<Entity*>& q)
{
	std::deque<Entity*> empty_q;
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
		turn_queue.push_back(e);
	}

	current_actor = turn_queue.front();
}

void TimeSystem::printTurnQueue()
{
	std::cout << "\n";
	std::deque<Entity*> temp_q = turn_queue;
	int count = 1;
	while (!temp_q.empty())
	{
		auto e = temp_q.front();
		auto c = e->getComponent<TimeComponent>();

		std::cout << count << ". " << e->getName() << " Speed: " << c->speed << " Build-up: " << c->built_up_speed << "\n";
		temp_q.pop_front();
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

void TimeSystem::rotateQueue()
{
	last_actor = turn_queue.front();
	turn_queue.pop_front();
	turn_queue.push_back(last_actor);
	current_actor = turn_queue.front();
}

void TimeSystem::onSpendTimeEvent(SpendTimeEvent* ev)
{
	auto time_comp = current_actor->getComponent<TimeComponent>();
	time_comp->built_up_speed -= ev->time;
}

