#pragma once
#include "Component.h"
#include "Event.h"

class Entity
{
private:
	// Data
	std::string name;
	std::map<std::type_index, std::unique_ptr<Component> > components;
	

public:
	// Data
	EventBus eventBus;

	// Functions
	Entity(std::string);
	
	std::string getName();
	
	void addComponent(Component * component);

	template<class T>
	bool removeComponent()
	{
		auto index = std::type_index(typeid(T));

		if (components.count(index))
		{
			auto c = components.at(index).get();

			eventBus.unsubscribe(c);

			components.erase(index);
			return true;
		}

		return false;
	}

	template<class T>
	Component* getComponent()
	{
		auto index = std::type_index(typeid(T));

		if (components.count(index))
		{
			return components.at(index).get();
		}

		return nullptr;
	}

	void fireEvent(Event& ev);
};