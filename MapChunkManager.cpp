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
		_chunk_status.insert({ coord, ChunkState::ACTIVE });
	}

	_th_fill_chunk_map = std::thread{ &MapChunkManager::fillChunkMap, this };
}

void MapChunkManager::update(sf::Vector2i world_position)
{
	updateChunkStates(world_position);
	setActiveChunks(world_position);
	updateInQueue(world_position);
	updateOutQueue(world_position);
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
}

MapChunk* MapChunkManager::buildChunk(sf::Vector2i world_position)
{
	auto chunk_ptr = _map_builder.build(world_position);
	auto coord = std::make_pair(world_position.x, world_position.y);

	_chunk_map_mutex.lock();
	_chunk_map.insert({coord, std::move(chunk_ptr) });
	_chunk_map_mutex.unlock();

	_chunk_status_mutex.lock();
	_chunk_status[coord] = ChunkState::LOADED;
	_chunk_status_mutex.unlock();

	return _chunk_map.at(coord).get();
}

MapChunk* MapChunkManager::loadChunk(sf::Vector2i world_position)
{
	return nullptr;
}

void MapChunkManager::saveChunk(sf::Vector2i world_position)
{

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

void MapChunkManager::updateInQueue(sf::Vector2i world_position)
{
}

void MapChunkManager::updateOutQueue(sf::Vector2i world_position)
{
	
}

void MapChunkManager::updateChunkStates(sf::Vector2i world_position)
{
	auto fuzzy_coords = getCoordsAboutCenter(world_position, 3 + 2);
	auto active_coords = getCoordsAboutCenter(world_position, 3);
	//set active coords and remove active from fuzzy
	for (auto& active : active_coords)
	{
		_chunk_status.at(active) = ChunkState::ACTIVE;
		for (auto& fuzzy : fuzzy_coords)
		{
			if (active == fuzzy)
			{
				fuzzy_coords.remove(fuzzy);
				break;
			}
		}
	}
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
				setState(fuzzy, ChunkState::TO_BUILD);
			}
			break;
		}
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
			if (_chunk_status.count(coord)) // check if it even has a status
			{
				// Need to build fresh
				buildChunk({ coord.first, coord.second });
			}
			else // if not, hasn't been built yet
			{
				buildChunk({ coord.first, coord.second });
			}
		}
		_build_queue_buffer.clear();
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
	_chunk_status_mutex.lock();

	_chunk_status[coord] = state;

	_chunk_status_mutex.unlock();
}

void MapChunkManager::printChunkStatus(sf::Vector2i world_position)
{
	auto coords = getCoordsAboutCenter(world_position, 3 + 2);
	int count = 0;
	int columns = 5;
	for (auto& coord : coords)
	{
		std::cout << "[" << status_enum_translator.at(_chunk_status.at(coord)) << "]\t";
		++count;
		if (count >= columns)
		{
			std::cout << "\n";
			count = 0;
		}
	}
	
}
