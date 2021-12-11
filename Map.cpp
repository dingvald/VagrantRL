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
}

std::list<Entity*>* Map::getEntitiesAt(unsigned int layer, sf::Vector2u position)
{
	return &entitiesAt[layer][position.x][position.y];
}

void Map::placeEntity(Entity* entity, unsigned int layer, sf::Vector2u position)
{
	entitiesAt[layer][position.x][position.y].push_back(entity);
}

void Map::removeEntity(Entity* entity, unsigned int layer, sf::Vector2u position)
{
	entitiesAt[layer][position.x][position.y].remove(entity);
}
