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

		EntityHandle dest(interactionEvent->destinationEntity, parentHub);

		if (dest.has<Faction>())
		{
			ComponentHandle<Faction> faction;

			parentHub->unpack(interactionEvent->destinationEntity, faction);

			switch (faction->faction)
			{
				case Faction::FRIENDLY:
				{

				}
				break;

				case Faction::ENEMY:
				{
					// publish an attack event
					eventBus->publish(new AttackEvent(interactionEvent->actingEntity, interactionEvent->destinationEntity));
				}
				break;

				case Faction::NEUTRAL:
				{

				}
				break;
			}
		}
		else
		{
			// Inanimate? Do nothing
			std::stringstream stream; // stream to create log messages
			stream << "There is a " << parentHub->getEntityName(interactionEvent->destinationEntity) << " in the way!";
			eventBus->publish(new LogEvent(stream.str(), sf::Color::White));
		}
	}

};

} // namespace ecs

