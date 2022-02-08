#include "pch.h"
#include "MapSystem.h"
#include "MapChunk.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), map_chunk_size.x,
		num_of_loaded_chunks.x, world);

	world->currentMap = map.get();

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

	/*
	for (int layer = 0; layer < (int)gl::Layer::Total; ++layer)
	{
		for (int x = starting_pos.x; x < starting_pos.x + num_of_loaded_chunks.x; ++x)
		{
			for (int y = starting_pos.y; y < starting_pos.y + num_of_loaded_chunks.y; ++y)
			{
				world->currentMap->addChunkToGrid(new MapChunk({ x,y }, map_chunk_size.x), layer);
			}
		}
	}
	*/

	
	auto player = world->addEntity("Player");
	world->setAsPlayer(player);
	player->addComponent(new PositionComponent({starting_pos.x*map_chunk_size.x + 32,starting_pos.y*map_chunk_size.y + 32}, gl::Layer::Actor));
	player->addComponent(new RenderComponent(0, sf::Color(100, 100, 100)));
	player->addComponent(new TimeComponent(100));
	player->addComponent(new PlayerAIComponent());
	player->addComponent(new ViewportFocusComponent());


	for (int i = 0; i < 100; ++i)
	{
		int x_rand = rand() % world->currentMap->getWidth();
		int y_rand = rand() % world->currentMap->getHeight();

		auto tree = world->addEntity("Tree");
		tree->addComponent(new PositionComponent({ starting_pos.x * map_chunk_size.x + x_rand, starting_pos.y * map_chunk_size.y + y_rand }, gl::Layer::Actor));
		tree->addComponent(new RenderComponent(5, sf::Color::Green));

	}


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

	// save chunks



	// save row/column of tiles + entities depending on direction
	world->currentMap->rotateMap(dir, 1);


	updateLoadedChunks();
	
	
	//printLoadedChunkCoords();
}

void MapSystem::updateLoadedChunks()
{
	sf::Vector2i center_pos = world->worldPosition;

	if (center_pos.x == 0) center_pos.x = 1;
	if (center_pos.y == 0) center_pos.y = 1;

	for (int x = center_pos.x - 1; x < center_pos.x - 1 + num_of_loaded_chunks.x; ++x)
	{
		for (int y = center_pos.y - 1; y < center_pos.y - 1 + num_of_loaded_chunks.y; ++y)
		{
			for (int layer = 0; layer < (int)gl::Layer::Total; ++layer)
			world->currentMap->addChunkToGrid(new MapChunk({ x,y }, map_chunk_size.x), layer);
		}
	}

	printLoadedChunkCoords();

	// get current world coord

	// get 9 coords based off current world coord

	// check which coords are already loaded

	// load/build chunks that are not yet loaded

	// get loaded but not used chunks


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
			if (!(x_int % (map_chunk_size.x * gl::TILE_SIZE)))
			{
				shiftActiveMap({ dir.x, 0 });
			}
		}
	}
	else
	{
		for (int x_int : x_ints)
		{
			if (!(x_int % (map_chunk_size.x * gl::TILE_SIZE - 1)))
			{
				shiftActiveMap({ dir.x, 0 });
			}
		}
	}

	if (dir.y == 1)
	{
		for (int y_int : y_ints)
		{
			if (!(y_int % (map_chunk_size.y * gl::TILE_SIZE)))
			{
				shiftActiveMap({ 0, dir.y });
			}
		}
	}
	else
	{
		for (int y_int : y_ints)
		{
			if (!(y_int % (map_chunk_size.y * gl::TILE_SIZE - 1)))
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

void MapSystem::printLoadedChunkCoords()
{
	std::cout << "Currently active chunks:\n";
	for (int y = 0; y < num_of_loaded_chunks.y; ++y)
	{
		for (int x = 0; x < num_of_loaded_chunks.x; ++x)
		{
			auto c = world->currentMap->getChunk(0, { x,y });

			std::cout << "(" << c->world_coordinate.x << ", " << c->world_coordinate.y << ")\n";
		}
	}
}
