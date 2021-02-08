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
		EntityHandle acting(interactionEvent->actingEntity, parentHub);

		if (dest.has<Faction>() && acting.has<Faction>())
		{
			ComponentHandle<Faction> faction_a, faction_d;

			parentHub->unpack(interactionEvent->destinationEntity, faction_d);
			parentHub->unpack(interactionEvent->actingEntity, faction_a);

			switch (calculateFaction(faction_a->faction, faction_d->faction))
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

	int calculateFaction(int faction1, int faction2)
	{
		if (faction1 == faction2) 
			return Faction::FRIENDLY;
		else
		{
			if (faction1 == Faction::NEUTRAL || faction2 == Faction::NEUTRAL)
			{
				return Faction::NEUTRAL;
			}
			else
			{
				return Faction::ENEMY;
			}
		}
	}

};

} // namespace ecs

