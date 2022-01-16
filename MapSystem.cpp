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
	player->addComponent(new PositionComponent({ 72,72 }, gl::Layer::Actor));
	player->addComponent(new RenderComponent(0, sf::Color(100, 100, 100)));
	player->addComponent(new TimeComponent(100));
	player->addComponent(new PlayerAIComponent());
	player->addComponent(new ViewportFocusComponent());

	for (int i = 0; i < 10000; ++i)
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
	// save row/column of tiles + entities depending on direction
	
	int x_start, x_end;
	int y_start, y_end;

	x_start = std::max(0, (int)(dir.x * (num_of_loaded_chunks.x - 1) * map_chunk_size.x));
	y_start = std::max(0, (int)(dir.y * (num_of_loaded_chunks.y - 1) * map_chunk_size.y));

}

void MapSystem::onViewportMoveEvent(ViewportMoveEvent* ev)
{
	// get viewport center
	sf::Vector2i center;

	center.x = ((int)ev->newOrigin.x + (gl::VIEWPORT_WIDTH * gl::TILE_SIZE) / 2) / gl::TILE_SIZE;
	center.y = ((int)ev->newOrigin.y + (gl::VIEWPORT_HEIGHT * gl::TILE_SIZE) / 2) / gl::TILE_SIZE;



}
