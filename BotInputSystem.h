#pragma once
#include "pch.h"
#include "ECS.h"

namespace ecs
{

class BotInputSystem : public System
{
private:

public:
	BotInputSystem()
	{

	}

	void init()
	{
		eventBus->subscribe(this, &BotInputSystem::onGetBotInputEvent);
	}

	void onGetBotInputEvent(GetBotInputEvent* bot)
	{
		// insert decision logic here

		sf::Vector2i dir = randomWalk();
		
		eventBus->publish(new MovementEvent(bot->entity, dir.x, dir.y));
	}

	sf::Vector2i randomWalk()
	{
		int x, y;
		x = (rand() % 3) - 1;
		y = (rand() % 3) - 1;

		return (sf::Vector2i(x, y));
	}
};

}// namespace ecs