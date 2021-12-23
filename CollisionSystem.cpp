#include "pch.h"
#include "CollisionSystem.h"

void CollisionSystem::init()
{
	eventBus->subscribe(this, &CollisionSystem::onCollisionEvent);
}



Relationship CollisionSystem::getRelationship(Entity* e1, Entity* e2)
{
// TODO in future: get the relationship using the faction system (?) 

	auto fac1 = e1->getComponent<FactionComponent>();
	auto fac2 = e2->getComponent<FactionComponent>();

	if (!fac1 || !fac2)
	{
		return Relationship::inanimate;
	}

	if (fac1->faction == fac2->faction)
	{
		return Relationship::friendly;
	}
	else
	{
		return Relationship::hostile;
	}

}


void CollisionSystem::onCollisionEvent(CollisionEvent* ev)
{
	Relationship relationship = getRelationship(ev->initiator, ev->receiver);

	switch (relationship)
	{
		case Relationship::friendly:
		{
			// Swap places
			eventBus->publish(new SwapPlacesEvent(ev->initiator, ev->receiver));
		}
		break;
		case Relationship::hostile:
		{
			// Attack!
			std::cout << ev->initiator->getName() << " attacks " << ev->receiver->getName() << "!\n";
		}
		break;
		case Relationship::inanimate:
		{
			// Do nothing
		}
		break;
		default:
		{
			// Do nothing
		}
		break;
	}
}
