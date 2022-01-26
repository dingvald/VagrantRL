#include "pch.h"
#include "Entity.h"
#include "Grid.h"
#include "Map.h"

Map::Map(unsigned int num_of_layers, unsigned int width, unsigned int height, World* world) : height(height), width(width), world(world)
{
	entities.resize(num_of_layers);
	for (unsigned int l = 0; l < num_of_layers; ++l)
	{
		entities[l].resize(width);
	}

	tiles.resize(width);
	for (unsigned int x = 0; x < width; ++x)
	{
		tiles[x].resize(height);
	}
}

std::list<Entity*>* Map::getEntitiesAt(unsigned int layer, sf::Vector2i position)
{
	if (position.x > width - 1 || position.y > height - 1 || position.x < 0 || position.y < 0)
	{
		return nullptr;
	}

	if (entities[layer].at(position.x,position.y)->empty())
	{
		return nullptr;
	}
	else
	{
		return entities[layer].at(position.x, position.y);
	}
	
}

void Map::applyFuncToEntitiesInRect(unsigned int x_start, unsigned int y_start, unsigned int rect_width, unsigned int rect_height,
	std::function<void(Entity*)> fun)
{

	if (x_start > width) return;
	if (y_start > height) return;

	if ((x_start + rect_width) > width)
	{
		rect_width = width - x_start;
	}
	if ((y_start + rect_height) > height)
	{
		rect_height = height - y_start;
	}

	for (unsigned int layer = 0; layer < (unsigned int)gl::Layer::Total; ++layer)
	{
		for (unsigned int x = x_start; x < x_start + rect_width; ++x)
		{
			for (unsigned int y = y_start; y < y_start + rect_height; ++y)
			{
				auto list = entities[layer].at(x,y);
				if (list->empty()) continue;

				for (auto e : *list)
				{
					fun(e);
				}
				
			}
		}
	}
}

void Map::placeEntity(Entity* entity, unsigned int layer, sf::Vector2i position)
{
	auto pos = world->worldToGridPosition(position);
	entities[layer].at(pos.x, pos.y)->push_back(entity);
}

void Map::removeEntity(Entity* entity, unsigned int layer, sf::Vector2i position)
{
	auto pos = world->worldToGridPosition(position);
	entities[layer].at(pos.x, pos.y)->remove(entity);
}

void Map::placeTile(Tile* tile, sf::Vector2u position)
{
	if (position.x < 0 || position.y < 0 || position.x > width || position.y > height)
	{
		return;
	}

	tiles[position.x][position.y] = std::unique_ptr<Tile>(tile);
}

Tile* Map::getTile(sf::Vector2i position)
{
	return tiles[position.x][position.y].get();
}

unsigned int Map::getWidth()
{
	return width;
}

unsigned int Map::getHeight()
{
	return height;
}

void Map::rotateMap(sf::Vector2i dir, int range)
{
	for (int layer = 0; layer < (int)gl::Layer::Total; ++layer)
	{
		entities[layer].shift(dir.x, dir.y, range);
	}
}