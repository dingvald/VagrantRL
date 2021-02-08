#pragma once
#include "pch.h"
#include "ECS.h"

namespace ecs
{

class TurnMultiplexer : public System
{
private:

public:

	void init()
	{
		eventBus->subscribe(this, &TurnMultiplexer::onStartTurnEvent);
	}

	void onStartTurnEvent(StartTurnEvent* turn)
	{
		ComponentHandle<AI> ai;

		parentHub->unpack(turn->entity, ai);

		switch (ai->ai_type)
		{
		case AI::PLAYER:
		{
			eventBus->publish(new GetPlayerInputEvent(turn->entity));
		}
		break;
		case AI::BOT:
		{
			eventBus->publish(new GetBotInputEvent(turn->entity));
		}
		break;
		}
	}
};

} // namespace ecs