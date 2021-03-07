#pragma once
#include "pch.h"
#include "Globals.h"
#include "ECS.h"
#include "EventBus.h"

class AIController
{
private:
	ecs::Entity entity;
	ecs::Hub* hub;
	EventBus* eventBus;

public:
	AIController(ecs::Entity entity, ecs::Hub* hub, EventBus* eventBus) : entity(entity), hub(hub), eventBus(eventBus) {};

	void update()
	{
		for (auto& ent : checkSurroundings(gl::BODIES))
		{
			ecs::EntityHandle eHandle(ent, hub);
			if (eHandle.has<ecs::Faction>())
			{
				ecs::ComponentHandle<ecs::Faction> f1, f2;
				hub->unpack(entity, f1);
				hub->unpack(ent, f2);
				if (ecs::InteractionSystem::calculateFaction(f1->faction, f2->faction) == ecs::Faction::ENEMY)
				{
					std::cout << "Enemy spotted!" << "\n";
				}
			}
		}

		eventBus->publish(new ActionEvent(100));
	}

	std::list<ecs::Entity> checkSurroundings(int layer)
	{
		std::list<ecs::Entity> surroundings;

		ecs::ComponentHandle<ecs::Vision> vision;
		ecs::ComponentHandle<ecs::Position> pos;
		hub->unpack(entity, vision, pos);

		sf::Vector2u surPos;
		std::map<unsigned int, sf::Vector2u>* list_of_pos = hub->getMap()->getListofPositions(layer);

		for (auto& surEntity : *list_of_pos)
		{
			surPos = surEntity.second;
			if ((std::abs((int)surPos.x - pos->x) < vision->radius) && (std::abs((int)surPos.y - pos->y) < vision->radius))
			{
				surroundings.push_back(hub->int2ent(surEntity.first));
			}
		}
		
		return surroundings;
	}
};


