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
		unsigned int id = ::getComponentID<C>();
		if (components.count(id))
		{
			world->removeComponent(this, id);
			eventBus.unsubscribe(components.at(id).get());
			components.erase(id);
		}
	}

	template <class C>
	C* getComponent()
	{
		unsigned int id = ::getComponentID<C>();
		if (components.count(id))
		{
			auto ptr = components.at(id).get();

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