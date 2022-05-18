#pragma once
#include "cereal/access.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/string.hpp"
#include "Serialization_Component.h"
#include "EventBus.h"
#include "World.h"
#include "ComponentMask.h"

class Entity
{
private:
	friend World;
	friend Component;
	friend class cereal::access;

	unsigned int id;
	std::string name;
	World* world = nullptr;
	ComponentMask signature;
	std::unordered_map<unsigned int, std::unique_ptr<Component> > components;
	
	
public:
	Entity() = default;
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

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(name, id, components);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		int num_comps = 0;
		std::vector<std::unique_ptr<Component>> comp_ptrs;

		archive(name, id, components);
		for (auto &c : components)
		{
			this->addComponent(c.second.get());
		}
	}
};