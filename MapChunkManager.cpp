#include "pch.h"
#include "MapChunkManager.h"
#include "World.h"
#include "Map.h"


/*
Order of chunk life
- Must be built / loaded
- Must be registered
- Made active once in range
*/



void MapChunkManager::init(World* world, sf::Vector2i world_position)
{
	this->_world = world;
	auto starting_coords = getCoordsAboutCenter(world_position, 3); // **Magic # 3 alert--Represents chunk load width
	for (auto& coord : starting_coords)
	{
		buildChunk({ coord.first, coord.second });
	}
	updateChunkStates(world_position);
	setActiveChunks(world_position);
	
	_process_in_queue = std::thread{ &MapChunkManager::th_buildOrLoadChunks, this };
	_process_out_queue = std::thread{ &MapChunkManager::th_saveChunks, this };
}

void MapChunkManager::update(sf::Vector2i world_position)
{
	updateChunkStates(world_position);
	setActiveChunks(world_position);
}

void MapChunkManager::cleanUp()
{
	_close_threads = true;
	_process_in_queue.join();
	_process_out_queue.join();
}

void MapChunkManager::activateChunk(MapChunk* map_chunk)
{
	auto coord = std::make_pair(map_chunk->world_coordinate.x, map_chunk->world_coordinate.y);
	_world->map->addChunkToGrid(map_chunk);
}

MapChunk* MapChunkManager::buildChunk(sf::Vector2i world_position)
{
	std::cout << "Building chunk...\n";
	auto chunk_ptr = _map_builder.build(world_position);
	auto coord = std::make_pair(world_position.x, world_position.y);

	_chunk_map_mutex.lock();
	_chunk_map.insert({coord, std::move(chunk_ptr) });
	_chunk_map_mutex.unlock();

	setState(coord, ChunkState::LOADED);

	return _chunk_map.at(coord).get();
}

MapChunk* MapChunkManager::loadChunk(sf::Vector2i world_position)
{
	std::cout << "Loading chunk...\n";
	return nullptr;
}

void MapChunkManager::saveChunk(sf::Vector2i world_position)
{
	std::cout << "Saving chunk...\n";
}

void MapChunkManager::setActiveChunks(sf::Vector2i world_position)
{
	auto active_coords = getCoordsAboutCenter(world_position, 3);
	for (auto& active_coord : active_coords)
	{
		assert(_chunk_map.at(active_coord) != nullptr);

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

void MapChunkManager::th_buildOrLoadChunks()
{
	while (!_close_threads)
	{
		// Build new Chunks
		std::deque<std::pair<int, int>> local_build_queue;
		_build_queue_mutex.lock();
		while (!_build_queue.empty())
		{
			local_build_queue.push_back(_build_queue.front());
			_build_queue.pop_front();
		}
		_build_queue_mutex.unlock();

		for (auto& coord : local_build_queue)
		{
			buildChunk({ coord.first, coord.second });
		}

		// Load Chunks from disk
		std::deque<std::pair<int, int>> local_load_queue;
		_load_queue_mutex.lock();
		while (!_load_queue.empty())
		{
			local_load_queue.push_back(_load_queue.front());
			_load_queue.pop_front();
		}
		_load_queue_mutex.unlock();

		for (auto& coord : local_build_queue)
		{
			loadChunk({ coord.first, coord.second });
		}
	}
}

void MapChunkManager::th_saveChunks()
{
	while (!_close_threads)
	{
		
	}
}

void MapChunkManager::updateChunkStates(sf::Vector2i world_position)
{
	auto fuzzy_coords = getCoordsAboutCenter(world_position, 3 + 2);
	auto active_coords = getCoordsAboutCenter(world_position, 3);

	//set active coords
	for (auto& active : active_coords)
	{
		setState(active, ChunkState::ACTIVE);

		// remove active coords from fuzzy coords
		for (auto& fuzzy : fuzzy_coords)
		{
			if (active == fuzzy)
			{
				fuzzy_coords.remove(fuzzy);
				break;
			}
		}
	}
	//change state of fuzzy coords
	for (auto& fuzzy : fuzzy_coords)
	{
		auto state = getState(fuzzy);
		
		switch (state)
		{
			case ChunkState::LOADED:
			{
				setState(fuzzy, ChunkState::LOADED);
			}
			break;
			case ChunkState::SAVED:
			{
				_load_queue_mutex.lock();
				_load_queue.push_back(fuzzy);
				_load_queue_mutex.unlock();
				setState(fuzzy, ChunkState::TO_LOAD);
			}
			break;
			case ChunkState::ACTIVE:
			{
				setState(fuzzy, ChunkState::LOADED);
			}
			break;
			case ChunkState::NONE:
			{
				_build_queue_mutex.lock();
				_build_queue.push_back(fuzzy);
				_build_queue_mutex.unlock();
				setState(fuzzy, ChunkState::TO_BUILD);
			}
			break;
		}
	}


	_fuzzy_coordinates = fuzzy_coords;
	_active_coordinates = active_coords;
}

ChunkState MapChunkManager::getState(std::pair<int, int> coord)
{
	ChunkState ret_state;

	_chunk_status_mutex.lock();
	if (_chunk_status.count(coord))
	{
		ret_state = _chunk_status.at(coord);
	}
	else
	{
		ret_state = ChunkState::NONE;
	}
	_chunk_status_mutex.unlock();


	return ret_state;
}

void MapChunkManager::setState(std::pair<int, int> coord, ChunkState state)
{
	auto old_state = getState(coord);

	if (old_state == state) return;
	if (old_state == ChunkState::NONE)
	{
		_chunk_status_mutex.lock();

		_chunk_state_lists[state].push_back(coord);
		_chunk_status[coord] = state;

		_chunk_status_mutex.unlock();
	}
	else
	{
		_chunk_status_mutex.lock();

		_chunk_state_lists.at(old_state).remove(coord);
		_chunk_state_lists[state].push_back(coord);
		_chunk_status[coord] = state;

		_chunk_status_mutex.unlock();
	}
	

	
}

void MapChunkManager::printChunkStatus(sf::Vector2i world_position)
{
	std::cout << "________________________________________________________________________________\n\n";
	
	std::cout << "WORK IN PROGRESS!\n";
	
	std::cout << "________________________________________________________________________________\n";
}
