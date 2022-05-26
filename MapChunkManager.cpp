#include "pch.h"
#include "MapChunkManager.h"
#include "World.h"

MapChunkManager::MapChunkManager(World* world)
{
	_world = world;
}

void MapChunkManager::init(sf::Vector2i world_position)
{
	auto starting_coords = getCoordsAboutCenter(world_position, 3); // **Magic # 3 alert--Represents chunk load width
	for (auto& coord : starting_coords)
	{
		auto chunk_ptr = _map_builder.build({coord.first, coord.second});
		_chunk_map.insert({ coord, std::move(chunk_ptr) });
		_chunk_status.insert({ coord, ChunkStatus::ACTIVE });
		// map->addChunkToGrid();
	}
}

void MapChunkManager::update(sf::Vector2i world_position)
{
}

void MapChunkManager::cleanUp()
{
}

std::list<std::pair<int, int>> MapChunkManager::getCoordsAboutCenter(sf::Vector2i world_position, int width)
{
	std::list<std::pair<int, int>> coord_list;

	if (!(width % 2))
	{
		std::cout << "ERROR (MapChunkManager.cpp): Choose an odd integer for the width...\n";
		width -= 1;
	}

	int offset = (width - 1) / 2;

	for (int x = world_position.x - offset; x < world_position.x - offset + width; ++x)
	{
		for (int y = world_position.y - offset; y < world_position.y - offset + width; ++y)
		{
			coord_list.push_back({ x,y });
		}
	}

	return coord_list;
}

void MapChunkManager::updateBuildQueue(sf::Vector2i world_position)
{
}

void MapChunkManager::fillChunkMap()
{
}
