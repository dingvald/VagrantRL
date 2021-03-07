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
		EntityHandle ent(turn->entity, parentHub);

		if (ent.has<Player>())
		{
			eventBus->publish(new GetPlayerInputEvent(turn->entity));
		}
		else
		{
			eventBus->publish(new GetAIInputEvent(turn->entity));
		}
	}
};

} // namespace ecs