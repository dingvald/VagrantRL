#pragma once
#include "pch.h"
#include "ECS.h"
#include "AIController.h"

namespace ecs
{

class AIInputSystem : public System
{
private:
	std::map< int, std::unique_ptr<AIController> > controllers;


public:
	AIInputSystem()
	{
		signature.addComponent<AI>();
	}

	void init()
	{
		eventBus->subscribe(this, &AIInputSystem::onGetAIInputEvent);
	}

	void onGetAIInputEvent(GetAIInputEvent* bot)
	{
		controllers.at(bot->entity.id)->update();
	}

	void update(const float dt)
	{
		// update the controllers map
		if (!(registeredEntities.size() == controllers.size()))
		{
			if (registeredEntities.size() < controllers.size())
			{
				bool found = false;

				for (auto it = controllers.begin(); it != controllers.end(); )
				{
					for (auto& entity : registeredEntities)
					{
						if ((*it).first == entity.id)
						{
							found = true;
							break;
						}
					}

					if (!found)
					{
						it = controllers.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			if (registeredEntities.size() > controllers.size())
			{
				for (auto& entity : registeredEntities)
				{
					// if registered entity is in the map
					if (controllers.count(entity.id))
					{
						// then move onto next entity
						continue;
					}
					// if registered entity is NOT in the map
					else
					{
						//then add entity to controller map 
						
						controllers.emplace(entity.id, std::make_unique<AIController>(entity, parentHub, eventBus));
						
					}
				}
			}

		}

	}
	

};

}// namespace ecs