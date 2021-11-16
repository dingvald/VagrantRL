#include "pch.h"
#include "Event.h"

// Initialize events with multiple parameters 
/* e.g.

	ev1 = Event(MeleeAttackEvent,
		{
			{"Damage", "11"},
			{"Type", "Blunt"},
			{"Anything", "Anything"}
		});

*/

Event::Event(EventType typeID, std::initializer_list<std::pair<const std::string, std::string>> params)
	: typeID(typeID)
{
	for (auto pair : params)
	{
		parameters.insert(std::make_pair(pair.first, pair.second));
	}
}