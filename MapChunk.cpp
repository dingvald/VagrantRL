#include "pch.h"
#include "MapChunk.h"
#include "Globals.h"

MapChunk::MapChunk(sf::Vector2i world_coordinate, int chunk_size)
	: world_coordinate(world_coordinate), size(chunk_size)
{
	entities.resize((int)gl::Layer::Total);
	for (int layer = 0; layer < (int)gl::Layer::Total; ++layer)
	{
		entities[layer].resize(size);
		for (int x = 0; x < size; ++x)
		{
			entities[layer][x].resize(size);
		}
	}
}

std::list<Entity*>* MapChunk::at(int layer, sf::Vector2i tile_position)
{
	if (tile_position.x > size || tile_position.y > size)
	{
		std::cout << "Indexing into chunk (" << tile_position.x << ", " << tile_position.y << ") is out of range...\n";
		return nullptr;
	}

	return &entities[layer][tile_position.x][tile_position.y];
}
