#include "pch.h"
#include "Entity.h"
#include "Map.h"

Map::Map(unsigned int num_of_layers, unsigned int width, unsigned int height) : height(height), width(width)
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

void Map::placeEntity(Entity* entity, unsigned int layer, sf::Vector2i position)
{
	auto pos = toGridPosition(position);
	entitiesAt[layer][pos.x][pos.y].push_back(entity);
}

void Map::removeEntity(Entity* entity, unsigned int layer, sf::Vector2i position)
{
	auto pos = toGridPosition(position);
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

sf::Vector2i Map::toGridPosition(sf::Vector2i position)
{
	position /= gl::TILE_SIZE;

	return position;
}
