#pragma once
#include "Entity.h"

// Forward declarations

//

class World
{
public:
	Entity* addEntity(std::string name);

	void removeEntity(Entity* entity);

	template<class C>
	std::list<Entity*> getEntitiesWith()
	{
		std::list<Entity*> list;
		for (auto& e : entities)
		{
			if (e->getComponent<C>() != nullptr)
			{
				list.push_back(e.get());
			}
		}

		return list;
	}

private:
	std::list< std::unique_ptr<Entity> > entities;
};

