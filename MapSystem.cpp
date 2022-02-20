#include "pch.h"
#include "MapSystem.h"
#include "MapChunk.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), gl::CHUNK_SIZE,
		num_of_loaded_chunks.x, world);

	world->map = map.get();

	eventBus->subscribe(this, &MapSystem::onViewportMoveEvent);

	buildInitialMap(starting_position);
}

void MapSystem::update(const float dt)
{
}

void MapSystem::buildInitialMap(sf::Vector2i starting_pos)
{
	world->worldPosition = starting_pos;

	//
	std::cout << "Building initial map (Location [" << world->worldPosition.x << ", " << world->worldPosition.y << "])..." << "\n";
	//

	updateLoadedChunks();
	printChunkBuffer();
	
	std::cout << "Creating player...\n";

	auto player = world->addEntity("Player");
	world->setAsPlayer(player);
	player->addComponent(new PositionComponent({starting_pos.x*gl::CHUNK_SIZE + 32,starting_pos.y*gl::CHUNK_SIZE + 32}, gl::Layer::Actor));
	player->addComponent(new RenderComponent(0, sf::Color(100, 100, 100)));
	player->addComponent(new TimeComponent(100));
	player->addComponent(new PlayerAIComponent());
	player->addComponent(new ViewportFocusComponent());

	std::cout << "Player complete.\n";
	/*
	for (int i = 0; i < 100; ++i)
	{
		int x_rand = rand() % world->map->getWidth();
		int y_rand = rand() % world->map->getHeight();

		auto tree = world->addEntity("Tree");
		tree->addComponent(new PositionComponent({ starting_pos.x * gl::CHUNK_SIZE + x_rand, starting_pos.y * gl::CHUNK_SIZE + y_rand }, gl::Layer::Actor));
		tree->addComponent(new RenderComponent(5, sf::Color::Green));

	}

	*/
	//
	std::cout << "Map complete." << "\n";
	//
}

void MapSystem::shiftActiveMap(sf::Vector2i dir)
{
	sf::Vector2i old_world_pos;
	old_world_pos.x = world->worldPosition.x;
	old_world_pos.y = world->worldPosition.y;

	world->worldPosition.x += dir.x;
	world->worldPosition.y += dir.y;

	sf::Vector2i rotate_dir = dir;
	
	if (world->worldPosition.x <= 1)
	{
		if (old_world_pos.x <= 1) dir.x = 0;
	}
	if (world->worldPosition.y <= 1)
	{
		if (old_world_pos.y <= 1) dir.y = 0;
	}

	world->map->shift(dir, 1);


	updateLoadedChunks();
	printLoadedChunkGrid();
}

void MapSystem::updateLoadedChunks()
{
	sf::Vector2i center_pos = world->worldPosition;

	if (center_pos.x == 0) center_pos.x = 1;
	if (center_pos.y == 0) center_pos.y = 1;

	std::list<std::pair<int, int>> old_coord_list = loaded_chunk_coords;
	loaded_chunk_coords.clear();

	for (int x = center_pos.x - 1; x < center_pos.x - 1 + num_of_loaded_chunks.x; ++x)
	{
		for (int y = center_pos.y - 1; y < center_pos.y - 1 + num_of_loaded_chunks.y; ++y)
		{
			loaded_chunk_coords.push_back({ x,y });
		}
	}

	for (auto coord : loaded_chunk_coords)
	{
		if (chunk_status.count(coord))
		{
			chunk_status.at(coord) = "Loaded";
			auto loaded_chunk = chunk_buffer.at(coord);
			world->map->addChunkToGrid(loaded_chunk.get());
		}
		else
		{
			std::shared_ptr<MapChunk> new_chunk(new MapChunk({ coord.first, coord.second }, gl::CHUNK_SIZE));
			chunk_status.insert({ coord, "Freshly Built" });
			chunk_buffer.insert({ coord, new_chunk});
			world->map->addChunkToGrid(new_chunk.get());
		}
	}	

	for (auto old_coord : old_coord_list)
	{
		bool isFound = false;
		for (auto coord : loaded_chunk_coords)
		{
			if (coord == old_coord) isFound = true;
		}
		if (!isFound)
		{
			chunk_status.at(old_coord) = "Saved";
			if (!chunk_buffer.count(old_coord))
			{
				std::cout << "ERROR! No chunk saved at location (" << old_coord.first << ", " << old_coord.second << ")\n";
			}
		}
	}
}

void MapSystem::loadOrBuildChunk(sf::Vector2i coords)
{
}

void MapSystem::calculateWorldMapPosition(sf::Vector2f new_viewport_pos, sf::Vector2f old_viewport_pos)
{
	sf::Vector2f origin = new_viewport_pos;
	sf::Vector2f old_origin = old_viewport_pos;
	sf::Vector2f fdiff = origin - old_origin;
	sf::Vector2i dir;
	//
	// Purpose is to determine whether the viewport center has crossed a map chunk boundary
	//

	// X direction
	if (fdiff.x > 0.0f)
	{
		dir.x = 1;

	}
	else if (fdiff.x < 0.0f)
	{
		dir.x = -1;
	}
	else
	{
		dir.x = 0;
	}
	// Y direction
	if (fdiff.y > 0.0f)
	{
		dir.y = 1;
	}
	else if (fdiff.y < 0.0f)
	{
		dir.y = -1;
	}
	else
	{
		dir.y = 0;
	}

	sf::Vector2f center;
	sf::Vector2f old_center;

	center.x = origin.x + (gl::VIEWPORT_WIDTH * gl::TILE_SIZE) / 2;
	center.y = origin.y + (gl::VIEWPORT_HEIGHT * gl::TILE_SIZE) / 2;
	old_center.x = old_origin.x + (gl::VIEWPORT_WIDTH * gl::TILE_SIZE) / 2;
	old_center.y = old_origin.y + (gl::VIEWPORT_HEIGHT * gl::TILE_SIZE) / 2;

	std::vector<int> x_ints;
	std::vector<int> y_ints;

	sf::Vector2f floored_center;
	sf::Vector2f floored_old_center;

	floored_center.x = std::floorf(center.x);
	floored_center.y = std::floorf(center.y);
	floored_old_center.x = std::floorf(old_center.x);
	floored_old_center.y = std::floorf(old_center.y);

	if (floored_center.x != floored_old_center.x)
	{
		int range = (int)std::fabs(floored_center.x - floored_old_center.x);
		for (int x = 0; x < range; ++x)
		{
			x_ints.push_back((int)floored_center.x + x);
		}
	}

	if (floored_center.y != floored_old_center.y)
	{
		int range = (int)std::fabs(floored_center.y - floored_old_center.y);
		for (int y = 0; y < range; ++y)
		{
			y_ints.push_back((int)floored_center.y + y);
		}
	}

	if (dir.x == 1)
	{

		for (int x_int : x_ints)
		{
			if (!(x_int % (gl::CHUNK_SIZE * gl::TILE_SIZE)))
			{
				shiftActiveMap({ dir.x, 0 });
			}
		}
	}
	else
	{
		for (int x_int : x_ints)
		{
			if (!(x_int % (gl::CHUNK_SIZE * gl::TILE_SIZE - 1)))
			{
				shiftActiveMap({ dir.x, 0 });
			}
		}
	}

	if (dir.y == 1)
	{
		for (int y_int : y_ints)
		{
			if (!(y_int % (gl::CHUNK_SIZE * gl::TILE_SIZE)))
			{
				shiftActiveMap({ 0, dir.y });
			}
		}
	}
	else
	{
		for (int y_int : y_ints)
		{
			if (!(y_int % (gl::CHUNK_SIZE * gl::TILE_SIZE - 1)))
			{
				shiftActiveMap({ 0, dir.y });
			}
		}
	}
}

void MapSystem::onViewportMoveEvent(ViewportMoveEvent* ev)
{
	calculateWorldMapPosition(ev->newOrigin, ev->oldOrigin);
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
		std::cout << "Confirrmed Coordinates: (" << chunk.second->world_coordinate.x << ", " << chunk.second->world_coordinate.y << ")\n";

		std::cout << "\n";
	}
	std::cout << "\n\n";
}
