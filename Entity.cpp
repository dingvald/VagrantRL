#include "pch.h"
#include "Entity.h"
#include "World.h"


Entity::Entity(std::string name)
	: name(name), world(nullptr)
{
}

Entity::Entity(const Entity& e)
{
	this->name = e.name;
	for (auto& c : e.components)
	{
		this->addComponent(c.second->clone());
	}
}

void Entity::operator=(const Entity& e)
{
	for (auto &c : e.components)
	{
		this->addComponent(c.second->clone());
	}
}

unsigned int Entity::getID() const
{
	return id;
}

std::string Entity::getName()
{
	return name;
}

void Entity::addComponent(Component* component)
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
		c->setOwnerTo(this);
		components.insert({ _id, std::move(c) });
		if (world)
		{
			world->addComponent(this, _id);
		}
		else
		{
			signature.addComponentByID(_id);
		}
	}
}
