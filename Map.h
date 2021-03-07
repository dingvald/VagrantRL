#pragma once
#include "pch.h"
#include "Globals.h"
#include "EventBus.h"



class Map
{
private:
	// map[x][y]
	std::vector< std::vector<gl::MapCell*> > map;
	std::map<unsigned int, sf::Vector2u> list_of_positions[gl::TOTAL_LAYERS];

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
		list_of_positions[layer].emplace(id, sf::Vector2u(x, y));
	}

	void removeObjectFromMap(int layer, int x, int y, int id)
	{
		std::list<gl::MapObject*>::iterator it;
		
		for (it = map[x][y]->objects[layer].begin(); it != map[x][y]->objects[layer].end(); ++it)
		{
			if ((*it)->id == id)
			{
				gl::MapObject* obj = *it;
				map[x][y]->objects[layer].remove(*it);
				delete obj;
				break;
			}
		}

		list_of_positions[layer].erase(id);
	}

	void moveMapObject(int layer, int from_x, int from_y, int to_x, int to_y)
	{
		// May introduce uninteded behaviour if the layer is not BODIES (may be wanting to move object NOT at the front of the list)

		gl::MapObject* object = map[from_x][from_y]->objects[layer].front();
		map[to_x][to_y]->objects[layer].push_back(object);
		map[from_x][from_y]->objects[layer].pop_front();

		// update the position list
		list_of_positions[layer].at(object->id) = sf::Vector2u(to_x, to_y);
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

	std::map<unsigned int, sf::Vector2u>* getListofPositions(int layer)
	{
		return &list_of_positions[layer];
	}

	gl::MapCell* cell(int x, int y)
	{
		return map[x][y];
	}

};