#pragma once
#include "World.h"
#include "EventBus.h"
#include "ComponentMask.h"
#include "cereal/access.hpp"

class Entity
{
public:
	unsigned int id;

private:
	friend World;
	friend Component;
	friend class cereal::access;

	// Data
	std::string name;
	std::map<unsigned int, std::unique_ptr<Component> > components;
	EventBus eventBus;
	World* world;
	ComponentMask signature;
	
public:
	Entity(std::string name);
	
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

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(name, id, components);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(name, id, components);
	}
};