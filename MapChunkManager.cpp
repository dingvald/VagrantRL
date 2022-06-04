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
	if (map_chunk == nullptr) std::cout << "\nCHUNK DOES NOT EXIST!\n\n";
	auto coord = std::make_pair(map_chunk->world_coordinate.x, map_chunk->world_coordinate.y);
	_world->map->addChunkToGrid(map_chunk);
}

MapChunk* MapChunkManager::buildChunk(const sf::Vector2i world_position)
{
	std::cout << "Building chunk\t\t(" << world_position.x << ", " << world_position.y << ")\n";
	auto chunk_ptr = _map_builder.build(world_position);
	auto coord = std::make_pair(world_position.x, world_position.y);

	_chunk_map_mutex.lock();
	_chunk_map.insert({coord, std::move(chunk_ptr) });
	_chunk_map_mutex.unlock();

	setState(coord, ChunkState::LOADED);

	return _chunk_map.at(coord).get();
}

MapChunk* MapChunkManager::loadChunk(const sf::Vector2i world_position)
{
	std::cout << "Loading chunk\t\t(" << world_position.x << ", " << world_position.y << ")\n";
	// waste some time to simulate loading...
	for (int i = 0; i < 10000; ++i)
	{
		for (int j = 0; j < 10000; ++j)
		{
			int sum = i + j;
		}
	}
	setState({ world_position.x, world_position.y }, ChunkState::LOADED);
	return nullptr;
}

void MapChunkManager::saveChunk(const sf::Vector2i world_position)
{
	std::cout << "Saving chunk\t\t(" << world_position.x << ", " << world_position.y << ")\n";
	// waste some time to simulate saving...
	for (int i = 0; i < 10000; ++i)
	{
		for (int j = 0; j < 10000; ++j)
		{
			int sum = i + j;
		}
	}
	setState({ world_position.x, world_position.y }, ChunkState::SAVED);
}

void MapChunkManager::setActiveChunks(const sf::Vector2i world_position)
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

std::list<std::pair<int, int>> MapChunkManager::getCoordsAboutCenter(const sf::Vector2i world_position, int width)
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

		for (auto& coord : local_load_queue)
		{
			loadChunk({ coord.first, coord.second });
		}
	}
}

void MapChunkManager::th_saveChunks()
{
	while (!_close_threads)
	{
		// save Chunks
		std::deque<std::pair<int, int>> local_save_queue;
		_save_queue_mutex.lock();
		while (!_save_queue.empty())
		{
			local_save_queue.push_back(_save_queue.front());
			_save_queue.pop_front();
		}
		_save_queue_mutex.unlock();

		for (auto& coord : local_save_queue)
		{
			saveChunk({ coord.first, coord.second });
		}
	}
}

void MapChunkManager::updateChunkStates(const sf::Vector2i world_position)
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


	auto loaded_list = _chunk_state_lists.at(ChunkState::LOADED);
	int coordx, coordy = 0;
	for (auto& coord : loaded_list)
	{
		coordx = coord.first;
		coordy = coord.second;
		int dx = abs(coordx - world_position.x);
		int dy = abs(coordy - world_position.y);

		auto dist = static_cast<int>(std::ceil(std::sqrtf(dx * dx + dy * dy)));

		if (dist > _save_distance)
		{
			setState(coord, ChunkState::TO_SAVE);
			_save_queue_mutex.lock();
			_save_queue.push_back(coord);
			_save_queue_mutex.unlock();
		}
	}
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
	auto coord_list = getCoordsAboutCenter(world_position, 5);
	std::deque<std::string> status_list;
	for (auto& coord : coord_list)
	{
		if (coord.first == world_position.x && coord.second == world_position.y)
		{
			status_list.push_back("   X   ");
		}
		else
		{
			status_list.push_back(status_enum_translator[getState(coord)]);
		}
	}
	int num_columns = 5;
	int column_counter = 0;
	int row_counter = 0;
	for (int i = 0; i < 25; ++i)
	{
		int index = row_counter + (column_counter * num_columns);

		std::cout << "[" << status_list[index] << "]\t";

		++column_counter;
		if (column_counter >= num_columns)
		{
			column_counter = 0;
			++row_counter;
			std::cout << "\n";
		}
		
	}

	
	std::cout << "________________________________________________________________________________\n";
}
