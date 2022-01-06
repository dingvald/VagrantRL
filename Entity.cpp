#include "pch.h"
#include "World.h"
#include "Entity.h"

Entity::Entity(std::string name)
	: name(name), world(nullptr)
{
}

std::string Entity::getName()
{
	return name;
}

void Entity::addComponent(Component * component)
{
	auto c = std::unique_ptr<Component>(component);
	auto _id = c->getID();

	if (components.count(_id))
	{
		std::cout << this->getName() << " already has Component" << _id << "!\n";
		return;
	}
	else
	{
		component->setOwnerTo(this);
		component->init();
		components.insert(std::make_pair(_id, std::move(c)));
		world->addComponent(this, _id);
	}

	
}



