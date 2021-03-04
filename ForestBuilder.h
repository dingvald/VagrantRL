#pragma once
#include "pch.h"
#include "Globals.h"
#include "ECS.h"

class ForestBuilder
{
private:
	ecs::Hub* ecsHub;
	int sparcity = 1;

public:

	ForestBuilder(ecs::Hub* hub) : ecsHub(hub) {};

	void setSparcity(int set_to)
	{
		sparcity = set_to;
	}

	void build()
	{
		for (int cell_x = 0; cell_x < gl::SCENE_WIDTH; ++cell_x)
		{
			for (int cell_y = 0; cell_y < gl::SCENE_HEIGHT; ++cell_y)
			{
				if (sparcity == 0) sparcity = 1;

				int placeHere = rand() % (sparcity);

				if (placeHere == 0 && !ecsHub->getMap()->checkForObject(gl::BODIES, cell_x, cell_y))
				{
					placeTree(cell_x, cell_y);
				}
			}
		}
	}

	void placeTree(int x, int y)
	{
		ecs::EntityHandle tree;
		tree = ecsHub->createEntity("Tree");
		tree.addComponent(ecs::Position(x, y, gl::Layer::BODIES));
		tree.addComponent(ecs::Sprite(5, sf::Color::Green));
		ecsHub->addEntityToMap(tree);
	}

};
