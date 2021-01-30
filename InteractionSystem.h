#pragma once
#include "ECS.h"


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
		std::stringstream stream;

		stream << "There is a " << parentHub->getEntityName(interactionEvent->destinationEntity) << " there!";
		eventBus->publish(new LogEvent(stream.str(), sf::Color::Red));
		
	}

};

} // namespace ecs

