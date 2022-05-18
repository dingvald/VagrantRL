#pragma once
#include "cereal/access.hpp"


// Forward declarations
class Entity;

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
	Component() = default;
	virtual ~Component() {};
	virtual unsigned int getID() = 0;
	virtual Component* clone() = 0;
	void setOwnerTo(Entity* entity);

protected:

	Entity* owner = nullptr;
};

template<class C>
struct ComponentID : public Component
{
	ComponentID() = default;
	virtual unsigned int getID()
	{
		static unsigned int thisid = counter();
		return thisid;
	}
	Component* clone() override;
};

template <class C>
static unsigned int getComponentID()
{
	ComponentID<C> temp;
	return temp.getID();
}

template<class C>
inline Component* ComponentID<C>::clone()
{
	return nullptr;
}
