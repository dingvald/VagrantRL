#pragma once
#include "World.h"
#include "EventBus.h"
#include "ComponentMask.h"

class Entity
{
private:
	friend World;
	friend Component;

	// Data
	std::string name;
	unsigned int id;
	std::map<unsigned int, std::unique_ptr<Component> > components;
	EventBus eventBus;
	World* world;
	ComponentMask signature;
	
public:
	// Data
	

	// Functions
	Entity(std::string);
	
	std::string getName();
	
	void addComponent(Component * component);

	template <class C>
	void removeComponent()
	{
		unsigned int _id = ::getComponentID<C>();
		if (components.count(_id))
		{
			world->removeComponent(this, _id);
			eventBus.unsubscribe(components.at(_id).get());
			components.erase(_id);
		}
	}

	template <class C>
	C* getComponent()
	{
		unsigned int _id = ::getComponentID<C>();
		if (components.count(_id))
		{
			auto ptr = components.at(_id).get();

			return static_cast<C*>(ptr);
		}

		return nullptr;
	}

	template<typename EventType>
	void fireEvent(std::shared_ptr<EventType> ev)
	{
		eventBus.publish(ev.get());
	}
};