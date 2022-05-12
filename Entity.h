#pragma once
#include "EventBus.h"
#include "World.h"
#include "ComponentMask.h"

class Entity
{
private:
	friend World;
	friend Component;

	unsigned int id;
	std::string name;
	World* world = nullptr;
	ComponentMask signature;
	std::unordered_map<unsigned int, std::unique_ptr<Component> > components;
	
	
public:
	Entity(std::string name);
	Entity(const Entity& e);

	void operator = (const Entity& e);

	unsigned int getID() const;
	
	std::string getName();
	
	void addComponent(Component* component);

	template <class C>
	void removeComponent()
	{
		unsigned int _id = ::getComponentID<C>();
		if (components.count(_id))
		{
			world->removeComponent(this, _id);
			components.erase(_id);
		}
	}

	template <class C>
	C* getComponent()
	{
		unsigned int _id = ::getComponentID<C>();
		auto it = components.find(_id);

		if (it != components.end())
		{
			auto ptr = it->second.get();

			return static_cast<C*>(ptr);
		}

		return nullptr;
	}
};