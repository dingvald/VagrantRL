#include "pch.h"
#include "Entity.h"
#include "Map.h"

Map::Map(unsigned int num_of_layers, unsigned int width, unsigned int height, World* world) : height(height), width(width), world(world)
{
	entitiesAt.resize(num_of_layers);
	for (unsigned int l = 0; l < num_of_layers; ++l)
	{
		entitiesAt[l].resize(width);
		for (unsigned int x = 0; x < width; ++x)
		{
			entitiesAt[l][x].resize(height);
		}
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

	if (entitiesAt[layer][position.x][position.y].empty())
	{
		return nullptr;
	}
	else
	{
		return &entitiesAt[layer][position.x][position.y];
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
				auto list = &entitiesAt[layer][x][y];
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
	entitiesAt[layer][pos.x][pos.y].push_back(entity);
}

void Map::removeEntity(Entity* entity, unsigned int layer, sf::Vector2i position)
{
	auto pos = world->worldToGridPosition(position);
	entitiesAt[layer][pos.x][pos.y].remove(entity);
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
		auto x_vec = &entitiesAt[layer];

		// rotate left when moving right...
		if (dir.x == 1)
		{
			std::rotate(x_vec->begin(), x_vec->begin() + range, x_vec->end());
		}
		// rotate right when moving left...
		else if (dir.x == -1)
		{
			std::rotate(x_vec->rbegin(), x_vec->rbegin() + range, x_vec->rend());
		}

		if (dir.y != 0)
		{
			int size = x_vec->size();

			for (int x = 0; x < size; ++x)
			{
				auto y_vec = &(*x_vec)[x];

				// rotate up when moving down
				if (dir.y == 1)
				{
					std::rotate(y_vec->begin(), y_vec->begin() + range, y_vec->end());
				}
				// rotate down when moving up
				else if (dir.y == -1)
				{
					std::rotate(y_vec->rbegin(), y_vec->rbegin() + range, y_vec->rend());
				}
			}
		}
	}
}