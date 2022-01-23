#include "pch.h"
#include "MapSystem.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), map_chunk_size.x * num_of_loaded_chunks.x,
		map_chunk_size.y * num_of_loaded_chunks.y);

	world->currentMap = map.get();

	eventBus->subscribe(this, &MapSystem::onViewportMoveEvent);

	buildInitialMap(starting_position);
}

void MapSystem::update(const float dt)
{
}

void MapSystem::buildInitialMap(sf::Vector2u starting_pos)
{
	world->worldPosition = starting_pos;

	//
	std::cout << "Building initial map (Location [" << world->worldPosition.x << ", " << world->worldPosition.y << "])..." << "\n";
	//
	auto player = world->addEntity("Player");
	world->setAsPlayer(player);
	player->addComponent(new PositionComponent({ 128,128 }, gl::Layer::Actor));
	player->addComponent(new RenderComponent(0, sf::Color(100, 100, 100)));
	player->addComponent(new TimeComponent(100));
	player->addComponent(new PlayerAIComponent());
	player->addComponent(new ViewportFocusComponent());

	for (int i = 0; i < 10; ++i)
	{
		int rand_x = rand() % world->currentMap->getWidth();
		int rand_y = rand() % world->currentMap->getHeight();

		auto npc = world->addEntity("Tree" + i);
		npc->addComponent(new PositionComponent({ rand_x,rand_y }, gl::Layer::Actor));
		npc->addComponent(new RenderComponent(5, sf::Color(75, 120, 50)));
	}
	//
	std::cout << "Map complete." << "\n";
	//
}

void MapSystem::shiftActiveMap(sf::Vector2i dir)
{
	world->worldPosition.x += dir.x;
	world->worldPosition.y += dir.y;

	// save row/column of tiles + entities depending on direction
	
	

}

void MapSystem::onViewportMoveEvent(ViewportMoveEvent* ev)
{

	sf::Vector2f origin = ev->newOrigin;
	sf::Vector2f old_origin = ev->oldOrigin;
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

	int x_int = -1;
	int y_int = -1;

	if (std::floorf(center.x) != std::floorf(old_center.x))
	{
		x_int = (int)std::floorf(center.x);
	}

	if (std::floorf(center.y) != std::floorf(old_center.y))
	{
		y_int = (int)std::floorf(center.y);
	}

	if (dir.x == 1)
	{
		if (!(x_int % (map_chunk_size.x * gl::TILE_SIZE)))
		{
			std::cout << "Crossing map chunk boundary at x = " << x_int << "\n";
			shiftActiveMap({ dir.x, 0 });
		}
	}
	else
	{
		if (!(x_int % (map_chunk_size.x * gl::TILE_SIZE -1)))
		{
			std::cout << "Crossing map chunk boundary at x = " << x_int << "\n";
			shiftActiveMap({ dir.x, 0 });
		}
	}

	if (dir.y == 1)
	{
		if (!(y_int % (map_chunk_size.y * gl::TILE_SIZE)))
		{
			//std::cout << "Crossing map chunk boundary at y = " << y_int << "\n";
			shiftActiveMap({ 0, dir.y });
		}
	}
	else
	{
		if (!(y_int % (map_chunk_size.y * gl::TILE_SIZE - 1)))
		{
			//std::cout << "Crossing map chunk boundary at y = " << y_int << "\n";
			shiftActiveMap({ 0, dir.y });
		}
	}
}
