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
		signature.addComponent<AI>();
		signature.addComponent<Vision>();
	}

	void init()
	{

	}

	void update(const float dt)
	{
		for (auto& entity : registeredEntities)
		{
			ComponentHandle<AI> ai;
			ComponentHandle<Vision> vision;
			ComponentHandle<Position> pos;

			parentHub->unpack(entity, ai, vision, pos);

			if (ai->ai_type == AI::PLAYER)
			{
				FOV fieldOfView(parentHub->getMap(), vision->radius);
				fieldOfView.update(pos->x, pos->y);
			}
		}
	}
};

}//namespace ecs