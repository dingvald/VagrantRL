#include "pch.h"
#include "MapSystem.h"
#include "MapChunk.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), gl::CHUNK_SIZE,
		num_of_loaded_chunks.x, world);

	world->map = map.get();

	buildInitialMap(starting_position);
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

	//
	std::cout << "Building initial map (Location [" << world->worldPosition.x << ", " << world->worldPosition.y << "])..." << "\n";
	//

	updateLoadedChunks();
	
	std::cout << "Creating player...\n";

	auto player = std::make_unique<Entity>("Player");
	player->addComponent(new PositionComponent({starting_pos.x*gl::CHUNK_SIZE + 32,starting_pos.y*gl::CHUNK_SIZE + 32}, gl::Layer::Actor));
	player->addComponent(new RenderComponent(0, sf::Color(100, 100, 100)));
	player->addComponent(new TimeComponent(100));
	player->addComponent(new PlayerAIComponent());
	player->addComponent(new ViewportFocusComponent()); 
	
	world->setAsPlayer(player.get());
	world->registerEntity(std::move(player));
	

	std::cout << "Player complete.\n";

	sf::Color col_array[3] = {
		sf::Color(8, 84, 34),
		sf::Color(20, 105, 21),
		sf::Color(17, 69, 38)
	};

	for (int i = 0; i < 10000; ++i)
	{
		int x_rand = (rand() % world->map->getWidth()) - gl::CHUNK_SIZE;
		int y_rand = (rand() % world->map->getHeight()) - gl::CHUNK_SIZE;
		int col_rand = rand() % 3;

		auto tree = std::make_unique<Entity>("Tree");
		tree->addComponent(new PositionComponent({ starting_pos.x * gl::CHUNK_SIZE + x_rand, starting_pos.y * gl::CHUNK_SIZE + y_rand }, gl::Layer::Actor));
		tree->addComponent(new RenderComponent(5, col_array[col_rand]));

		world->registerEntity(std::move(tree));
	}
	//
	std::cout << "Map complete." << "\n";
	//
}

void MapSystem::updateLoadedChunks()
{
	sf::Vector2i center_pos = world->worldPosition;

	if (center_pos.x == 0) center_pos.x = 1;
	if (center_pos.y == 0) center_pos.y = 1;

	updateChunkStatus(old_world_position, center_pos);
	printChunkStatus();

	for (int x = center_pos.x - 1; x < center_pos.x - 1 + num_of_loaded_chunks.x; ++x)
	{
		for (int y = center_pos.y - 1; y < center_pos.y - 1 + num_of_loaded_chunks.y; ++y)
		{
			if (chunk_status.count({x,y}))
			{
				auto loaded_chunk = chunk_buffer.at({x,y});
				world->map->addChunkToGrid(loaded_chunk.get(),{x - center_pos.x + 1, y - center_pos.y + 1});
			}
			else
			{
				std::shared_ptr<MapChunk> new_chunk(new MapChunk({ x,y }, gl::CHUNK_SIZE));
				chunk_status.insert({ {x,y}, "Freshly Built" });
				chunk_buffer.insert({ {x,y}, new_chunk });
				world->map->addChunkToGrid(new_chunk.get(), {x - center_pos.x + 1, y - center_pos.y + 1});
			}
		}
	}
}

std::list<std::pair<int, int>> MapSystem::getCoordsAboutCenter(sf::Vector2i center_pos)
{
	std::list<std::pair<int, int>> coord_list;

	for (int x = center_pos.x - 1; x < center_pos.x - 1 + num_of_loaded_chunks.x; ++x)
	{
		for (int y = center_pos.y - 1; y < center_pos.y - 1 + num_of_loaded_chunks.y; ++y)
		{
			coord_list.push_back({ x,y });
		}
	}

	return coord_list;
}

void MapSystem::updateChunkStatus(sf::Vector2i old_center, sf::Vector2i new_center)
{
	auto current_coords = getCoordsAboutCenter(new_center);
	auto old_coords = getCoordsAboutCenter(old_center);

	// Mutex lock?

	for (auto old_coord : old_coords)
	{
		if (chunk_status.count(old_coord))
		{
			chunk_status.at(old_coord) = "Inactive";
		}
		else
		{
			std::cout << "ERROR: Somehow this old_coord is not in the status map...\n";
		}
	}

	for (auto coord : current_coords)
	{
		if (chunk_status.count(coord))
		{
			if (chunk_status.at(coord) == "Inactive")
			{
				// Grab from buffer

				// Set to "Active"
				chunk_status.at(coord) = "Active";
			}
			else if (chunk_status.at(coord) == "Active")
			{
				// Do nothing.... I think...
			}
		}
		else
		{
			std::cout << "ERROR: Trying to make un built chunk active... thread loading too slow!!!\n";
			// Maybe insert an emergency Load/build in certain cases i.e teleportation?
		}
	}

	// Mutex unlock?
}

void MapSystem::printChunkStatus()
{
	std::cout << "Chunk Status:\n";

	for (auto c : chunk_status)
	{
		std::cout << c.first.first << ", " << c.first.second << ": " << c.second << "\n";
	}
	
}

void MapSystem::printLoadedChunkGrid()
{
	std::cout << "Loaded chunk layout:\n\n";

	auto map = world->map;

	for (int y = 0; y < num_of_loaded_chunks.y; ++y)
	{
		for (int x = 0; x < num_of_loaded_chunks.x; ++x)
		{
			auto chunk = map->getChunk({ x,y });
			std::cout << "(" << chunk->world_coordinate.x << ", " << chunk->world_coordinate.y << ")\t\t";
		}
		std::cout << "\n";
	}
}

void MapSystem::printChunkBuffer()
{
	std::cout << "Chunk Buffer:\n\n";
	for(auto chunk : chunk_buffer)
	{
		std::cout << "Coordinates: (" << chunk.first.first << ", " << chunk.first.second << ")\n";
		std::cout << "Confirmed Coordinates: (" << chunk.second->world_coordinate.x << ", " << chunk.second->world_coordinate.y << ")\n";

		std::cout << "\n";
	}
	std::cout << "\n\n";
}
