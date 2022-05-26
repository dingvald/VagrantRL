#include "pch.h"
#include "MapSystem.h"
#include "MapChunk.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), gl::CHUNK_SIZE,
		num_of_loaded_chunks.x, world);

	world->map = map.get();

	buildInitialMap(starting_position);
	fill_buffer_thread = std::thread{ &MapSystem::fillChunkBuffer, this };
}

void MapSystem::cleanUp()
{
	// Wait for thread to close
	close_buffer_thread = true;
	fill_buffer_thread.join();
}

void MapSystem::update(const float dt)
{
	if (world_position != world->worldPosition)
	{
		old_world_position = world_position;
		world_position = world->worldPosition;
		updateLoadedChunks();
	}
}

void MapSystem::buildInitialMap(sf::Vector2i starting_pos)
{
	world->worldPosition = starting_pos;
	world_position = starting_pos;
	old_world_position = starting_pos;

	//
	std::cout << "Building initial map (Location [" << world->worldPosition.x << ", " << world->worldPosition.y << "])..." << "\n";
	//
	std::cout << "Building chunks...\n";
	auto starting_coords = getCoordsAboutCenter(starting_pos, num_of_loaded_chunks.x);
	for (auto coord : starting_coords)
	{
		auto chunk = mapBuilder.build({ coord.first, coord.second });
		chunk_buffer.insert({ coord, chunk });
		map->addChunkToGrid(chunk.get(), { coord.first - starting_pos.x + 1, coord.second - starting_pos.y + 1});
	}
	std::cout << "Chunks complete.\n";

	updateLoadedChunks();

	std::cout << "Creating player...\n";

	auto player = std::make_unique<Entity>("Player");
	player->addComponent(new PositionComponent({starting_pos.x*gl::CHUNK_SIZE + 32,starting_pos.y*gl::CHUNK_SIZE + 32}, gl::Layer::Actor));
	player->addComponent(new RenderComponent(0, sf::Color(100, 100, 100)));
	player->addComponent(new TimeComponent(100));
	player->addComponent(new PlayerAIComponent());
	player->addComponent(new CameraFocusComponent()); 
	
	auto player_ptr = world->registerEntity(std::move(player));
	world->setAsPlayer(player_ptr);
}

void MapSystem::updateLoadedChunks()
{
	sf::Vector2i center_pos = world->worldPosition;

	if (center_pos.x == 0) center_pos.x = 1;
	if (center_pos.y == 0) center_pos.y = 1;

	updateChunkBuildQ(old_world_position, center_pos);
}

std::list<std::pair<int, int>> MapSystem::getCoordsAboutCenter(sf::Vector2i center_pos, int width)
{
	std::list<std::pair<int, int>> coord_list;

	if (!(width % 2))
	{
		std::cout << "ERROR (MapSystem.cpp): Choose an odd integer for the width...\n";
		width -= 1;
	}

	int offset = (width - 1) / 2;

	for (int x = center_pos.x - offset; x < center_pos.x - offset + width; ++x)
	{
		for (int y = center_pos.y - offset; y < center_pos.y - offset + width; ++y)
		{
			coord_list.push_back({ x,y });
		}
	}

	return coord_list;
}

void MapSystem::updateChunkBuildQ(sf::Vector2i old_center, sf::Vector2i new_center)
{
	auto active_coords = getCoordsAboutCenter(new_center, num_of_loaded_chunks.x);
	auto fuzzy_load_coords = getCoordsAboutCenter(new_center, num_of_loaded_chunks.x + 2);

	for (auto fuzzy : fuzzy_load_coords)
	{
		for (auto coord : active_coords)
		{
			chunk_buffer_lock.lock();
			auto chunk = chunk_buffer.at(coord);
			chunk_buffer_lock.unlock();
			map->addChunkToGrid(chunk.get(), { coord.first - new_center.x + 1, coord.second - new_center.y + 1 });

			if (fuzzy == coord) continue;

			build_queue_lock.lock();
			build_queue.push_back(fuzzy);
			build_queue_lock.unlock();
		}
	}
}

void MapSystem::fillChunkBuffer()
{
	while (!close_buffer_thread)
	{
		build_queue_lock.lock();
		while (!build_queue.empty())
		{
			build_queue_buffer.push_back(build_queue.front());
			build_queue.pop_front();
		}
		build_queue_lock.unlock();

		for (auto coord : build_queue_buffer)
		{
			if (chunk_buffer.count(coord))
			{
				if (chunk_buffer.at(coord) != nullptr) continue;
				auto chunk_ptr = mapBuilder.build({ coord.first, coord.second });
				chunk_buffer_lock.lock();
				chunk_buffer.at(coord) = chunk_ptr;
				chunk_buffer_lock.unlock();
			}
			else
			{
				auto chunk_ptr = mapBuilder.build({ coord.first, coord.second });
				chunk_buffer_lock.lock();
				chunk_buffer.insert({ coord, chunk_ptr });
				chunk_buffer_lock.unlock();
			}
			std::cout << "Built chunk (" << coord.first << ", " << coord.second << ")\n";
		}
	}
}
