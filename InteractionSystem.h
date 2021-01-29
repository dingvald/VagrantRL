#pragma once
#include "System.h"


namespace ecs
{

class InteractionSystem : public System
{
private:

public:
	InteractionSystem()
	{

	}

	void init()
	{
		eventBus->subscribe(this, &InteractionSystem::onInteractionEvent);
	}

	void onInteractionEvent(InteractionEvent* interactionEvent)
	{
		// Depending on relationship between the two entities, route interaction to another system
		
	}

};

} // namespace ecs

