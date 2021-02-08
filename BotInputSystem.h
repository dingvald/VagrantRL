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
		eventBus->publish(new ActionEvent(100));
	}
};

}// namespace ecs