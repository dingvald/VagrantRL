#pragma once
#include "pch.h"
#include "Globals.h"
#include "ECS.h"

#include "FOV.h"


namespace ecs
{

class FOVSystem : public System
{
private:

public:
	FOVSystem()
	{
		signature.addComponent<Vision>();
	}

	void init()
	{

	}

	void update(const float dt)
	{
		for (auto& entity : registeredEntities)
		{
			ComponentHandle<Vision> vision;
			ComponentHandle<Position> pos;
			EntityHandle ent(entity, parentHub);

			parentHub->unpack(entity, vision, pos);

			if (ent.has<Player>())
			{
				FOV fieldOfView(parentHub->getMap(), vision->radius);
				fieldOfView.update(pos->x, pos->y);
			}
		}
	}
};

}//namespace ecs