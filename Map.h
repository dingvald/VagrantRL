#pragma once
#include "pch.h"
#include "Globals.h"
#include "EventBus.h"



class Map
{
private:
	// map[x][y]
	std::vector< std::vector<gl::MapCell*> > map;

	EventBus *eventBus;

public:

	Map(EventBus* eventBus) : eventBus(eventBus)
	{

	}

	void init()
	{
		map.resize(gl::SCENE_WIDTH);
		for (int x = 0; x < gl::SCENE_WIDTH; ++x)
		{
			map[x].resize(gl::SCENE_HEIGHT);
			for (int y = 0; y < gl::SCENE_HEIGHT; ++y)
			{
				map[x][y] = new gl::MapCell();
			}
		}
	}

	void addObjectToMap(int layer, int x, int y, unsigned int id, unsigned int sprite, sf::Color color, bool blocks_light)
	{
		map[x][y]->objects[layer].push_back(new gl::MapObject(id, sprite, color, blocks_light));
	}

	void removeObjectFromMap(int layer, int x, int y, int id)
	{
		std::list<gl::MapObject*>::iterator it;
		
		for (it = map[x][y]->objects[layer].begin(); it != map[x][y]->objects[layer].end(); ++it)
		{
			if ((*it)->id == id)
			{
				map[x][y]->objects[layer].remove(*it);
				break;
			}
		}
	}

	void moveMapObject(int layer, int from_x, int from_y, int to_x, int to_y)
	{
		gl::MapObject* object = map[from_x][from_y]->objects[layer].front();
		map[to_x][to_y]->objects[layer].push_back(object);
		map[from_x][from_y]->objects[layer].pop_front();
	}

	gl::MapObject* checkForObject(int layer, int x, int y)
	{
		if (!map[x][y]->objects[layer].empty())
		{
			return map[x][y]->objects[layer].front();
		}
		else
		{
			return nullptr;
		}
	}

	void clearVisibility()
	{
		for (int x = 0; x < gl::SCENE_WIDTH; ++x)
		{
			for (int y = 0; y < gl::SCENE_HEIGHT; ++y)
			{
				map[x][y]->isVisible = false;
			}
		}
	}

	std::vector< std::vector< gl::MapCell* > >* getCellArray()
	{
		return &map;
	}

	gl::MapCell* cell(int x, int y)
	{
		return map[x][y];
	}

};