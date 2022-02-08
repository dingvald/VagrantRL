#include "pch.h"
#include "MapChunk.h"

MapChunk::MapChunk(sf::Vector2i world_coordinate, int chunk_size)
	: world_coordinate(world_coordinate), size(chunk_size)
{
	entities.resize(size);
	for (int x = 0; x < size; ++x)
	{
		entities[x].resize(size);
	}
}

std::list<Entity*>* MapChunk::at(sf::Vector2i tile_position)
{
	if (tile_position.x > size || tile_position.y > size)
	{
		std::cout << "Indexing into chunk (" << tile_position.x << ", " << tile_position.y << ") is out of range...\n";
		return nullptr;
	}

	return &entities[tile_position.x][tile_position.y];
}
