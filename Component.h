#pragma once



// Forward declarations
class Entity;
class Event;
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
	virtual ~Component() {};
	virtual void init() = 0;
	virtual unsigned int getID() = 0;
	void setOwnerTo(Entity* entity);

protected:
	Entity* owner;
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