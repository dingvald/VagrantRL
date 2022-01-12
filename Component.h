#pragma once
#include "cereal/access.hpp"


// Forward declarations
class Entity;
class EventBus;

//

// Component Base Class //////////////////////////////////////////////////////////////////////////////////////

inline unsigned int counter()
{
	static unsigned int count = 0;
	return ++count;
}

class Component
{
public:
	friend class cereal::access;

	virtual ~Component() {};
	virtual void init() = 0;
	virtual unsigned int getID() = 0;
	void setOwnerTo(Entity* entity);

protected:

	Entity* owner;
	unsigned int owner_id = { 0 };
	EventBus* eventBus;
};

template<class C>
struct ComponentID : public Component
{
	virtual void init() {};

	virtual unsigned int getID()
	{
		static unsigned int thisid = counter();
		return thisid;
	}
};

template <class C>
static unsigned int getComponentID()
{
	ComponentID<C> temp;
	return temp.getID();
}