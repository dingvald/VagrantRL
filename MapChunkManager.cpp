#include "pch.h"
#include "MapChunkManager.h"
#include "World.h"
#include "Map.h"

void MapChunkManager::init(World* world, sf::Vector2i world_position)
{
	this->_world = world;
	auto starting_coords = getCoordsAboutCenter(world_position, 3); // **Magic # 3 alert--Represents chunk load width
	for (auto& coord : starting_coords)
	{
		auto chunk_ptr = _map_builder.build({coord.first, coord.second});
		activateChunk(chunk_ptr.get());
		_chunk_map.insert({ coord, std::move(chunk_ptr) });
		_chunk_status.insert({ coord, ChunkStatus::ACTIVE });
	}

	updateBuildQueue(world_position);
	_th_fill_chunk_map = std::thread{ &MapChunkManager::fillChunkMap, this };
}

void MapChunkManager::update(sf::Vector2i world_position)
{
	setActiveChunks(world_position);
	updateBuildQueue(world_position);
}

void MapChunkManager::cleanUp()
{
	_close_buffer_thread = true;
	_th_fill_chunk_map.join();
}

void MapChunkManager::activateChunk(MapChunk* map_chunk)
{
	auto coord = std::make_pair(map_chunk->world_coordinate.x, map_chunk->world_coordinate.y);
	_world->map->addChunkToGrid(map_chunk);

	_chunk_status_mutex.lock();
	_chunk_status[coord] = ChunkStatus::ACTIVE;
	_chunk_status_mutex.unlock();
}

void MapChunkManager::setActiveChunks(sf::Vector2i world_position)
{
	auto active_coords = getCoordsAboutCenter(world_position, 3);
	for (auto& active_coord : active_coords)
	{
		_chunk_map_mutex.lock();
		auto chunk_ptr = _chunk_map.at(active_coord).get();
		activateChunk(chunk_ptr);
		_chunk_map_mutex.unlock();
	}
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
	
	auto fuzzy_coords = getCoordsAboutCenter(world_position, 3 + 2);

	for (auto& fuzzy_coord : fuzzy_coords)
	{
		if (_chunk_status.count(fuzzy_coord)) continue;

		_build_queue_mutex.lock();
		_build_queue.push_back(fuzzy_coord);
		_build_queue_mutex.unlock();

		_chunk_status_mutex.lock();
		_chunk_status[fuzzy_coord] = ChunkStatus::FUZZY_IN;
		_chunk_status_mutex.unlock();
	}	
}

void MapChunkManager::fillChunkMap()
{
	while (!_close_buffer_thread)
	{
		_build_queue_mutex.lock();
		while (!_build_queue.empty())
		{
			_build_queue_buffer.push_back(_build_queue.front());
			_build_queue.pop_front();
		}
		_build_queue_mutex.unlock();

		for (auto& coord : _build_queue_buffer)
		{
			if (_chunk_status.count(coord))
			{
				
			}
			else
			{
				// Need to build it fresh
				auto chunk_ptr = _map_builder.build({ coord.first, coord.second });
				_chunk_map_mutex.lock();
				_chunk_map.insert({ coord, std::move(chunk_ptr) });
				_chunk_map_mutex.unlock();

				_chunk_status_mutex.lock();
				_chunk_status[coord] = ChunkStatus::LOADED;
				_chunk_status_mutex.unlock();
			}
		}
	}
}

void MapChunkManager::printChunkStatus()
{
	for (auto& status : _chunk_status)
	{
		std::cout << "Chunk (" << status.first.first << ", " << status.first.second << ") Status: " << status_enum_translator.at(status.second) << "\n";
	}
}
