#include "pch.h"
#include "Event.h"
#include "Entity.h"

Entity::Entity(std::string name)
	: name(name)
{
}

std::string Entity::getName()
{
	return name;
}

void Entity::addComponent(Component * component)
{
	component->setOwnerTo(this);
	component->init();

	auto index = std::type_index(typeid(*component));

	if (components.count(index))
	{
		std::cout << this->getName() << " already has " << index.name() << "!\n";
	}

	components.insert(std::make_pair(index, std::unique_ptr<Component>(component)));
}

void Entity::fireEvent(Event& ev)
{
	eventBus.publish(ev);
}



