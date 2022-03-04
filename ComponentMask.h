#pragma once
#include "Globals.h"
#include "Component.h"

class ComponentMask
{
public:

	template<class C>
	void addComponentByType()
	{
		mask.set(::getComponentID<C>());
	}

	template<class C>
	void removeComponentByType()
	{
		mask.reset(::getComponentID<C>());
	}

	void addComponentByID(unsigned int id);

	void removeComponentByID(unsigned int id);

	bool isNewMatch(ComponentMask oldMask, ComponentMask systemMask);

	bool noLongerMatches(ComponentMask oldMask, ComponentMask systemMask);

private:
	std::bitset<gl::COMPONENT_MAX> mask = {0};

	bool matches(ComponentMask otherMask);
	
};

