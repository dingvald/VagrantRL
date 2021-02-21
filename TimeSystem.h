#pragma once
#include "ECS.h"


namespace ecs
{


class TimeSystem : public System
{
private:
	std::deque<int> turn_queue;

public:
	TimeSystem()
	{
		signature.addComponent<Time>();
	}

	void init()
	{
		eventBus->subscribe(this, &TimeSystem::onActionEvent);

		turn_queue.push_front(0);
	}

	void update(const float dt)
	{
		// update the turn queue

		if (turn_queue.size() < registeredEntities.size() + 1)
		{
			for (auto& entity : registeredEntities)
			{
				bool found = false;
				for (int i = 0; i < turn_queue.size(); ++i)
				{
					if (entity.id == turn_queue[i])
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					turn_queue.push_back(entity.id);
				}
			}
		}

		if (turn_queue.size() > registeredEntities.size() + 1)
		{
			std::deque<int>::iterator it = turn_queue.begin();

			//search turn queue for registered entities
			while (it != turn_queue.end())
			{
				if (*it == 0)
				{
					++it;
					continue;
				}

				bool found = false;
				for (auto& entity : registeredEntities)
				{
					//if the entity in the turn queue is registered
					if (*it == entity.id)
					{
						found = true;
						//go on to the next entity in the turn queue
						++it;
						break;
					}
				}
				//if the entity in the turn queue is not registered
				if (!found)
				{
					//remove the entity from the turn queue
					it = turn_queue.erase(it);
				}
			}
		}

		if (turn_queue.front() == 0)
		{
			// Once the queue has cycled once, refresh the action points

			for (int i = 1; i < turn_queue.size(); ++i)
			{
				ComponentHandle<Time> time_handle;

				Entity temp;
				temp.id = turn_queue[i];
				parentHub->unpack(temp, time_handle);

				time_handle->actionPoints += time_handle->speed;
			}

			turn_queue.pop_front();
			turn_queue.push_back(0);
		}
		else
		{
			ComponentHandle<Time> time_handle;

			Entity temp;

			temp.id = turn_queue.front();
			parentHub->unpack(temp, time_handle);

			if (time_handle->actionPoints > 0)
			{
				eventBus->publish(new StartTurnEvent(temp));
			}
			else
			{
				turn_queue.pop_front();
				turn_queue.push_back(temp.id);
			}
		}
	}

	void onActionEvent(ActionEvent* action)
	{
		ComponentHandle<Time> time_handle;

		Entity temp;
		temp.id = turn_queue.front();
		parentHub->unpack(temp, time_handle);

		time_handle->actionPoints -= action->cost;

		if (time_handle->actionPoints > 0)
		{
			eventBus->publish(new StartTurnEvent(temp));
		}
		else
		{
			turn_queue.pop_front();
			turn_queue.push_back(temp.id);
		}
	}

};


} // namespace ecs
