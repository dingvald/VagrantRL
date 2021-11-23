#include "pch.h"
#include "ComponentMask.h"

void ComponentMask::addComponentByID(unsigned int id)
{
	mask.set(id);
}

void ComponentMask::removeComponentByID(unsigned int id)
{
	mask.reset(id);
}

bool ComponentMask::isNewMatch(ComponentMask oldMask, ComponentMask systemMask)
{

	return ( matches(systemMask) && !oldMask.matches(systemMask) );
}

bool ComponentMask::noLongerMatches(ComponentMask oldMask, ComponentMask systemMask)
{
	return (oldMask.matches(systemMask) && !matches(systemMask));
}

bool ComponentMask::matches(ComponentMask otherMask)
{
	if ((mask & otherMask.mask) == otherMask.mask)
	{
		return true;
	}
	return false;
}
