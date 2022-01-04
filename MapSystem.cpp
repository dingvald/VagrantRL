#include "pch.h"
#include "MapSystem.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), map_chunk_size.x * 3, map_chunk_size.y * 3);
	world->currentMap = map.get();

	buildInitialMap();

	std::cout << "Map width: " << map->getWidth() << "\n";
	std::cout << "Map height: " << map->getHeight() << "\n";
}

void MapSystem::buildInitialMap()
{
	// Build tile layer
	for (unsigned int x = 0; x < map->getWidth(); ++x)
	{
		for (unsigned int y = 0; y < map->getHeight(); ++y)
		{
			map->placeTile(new Tile(2, sf::Color{ 100,100,100 }, sf::Color::Black), { x,y });
		}
	}
	// Add entities

	auto player = world->addEntity("Player");
	player->addComponent(new TestComponent());
	player->addComponent(new ViewportFocusComponent);
	player->addComponent(new RenderComponent(0, sf::Color{ 100, 100, 100 }));
	player->addComponent(new PositionComponent(2, 16, gl::Layer::Actor));
	player->addComponent(new HealthComponent(10));
	player->addComponent(new TimeComponent(100));
	player->addComponent(new PlayerAIComponent);
	player->addComponent(new FactionComponent(Faction::goodguys));

	for (int x = 0; x < 5; ++x)
	{
		int rand_x, rand_y;

		rand_x = (rand() % ((map_chunk_size.x*3)-1));
		rand_y = (rand() % ((map_chunk_size.y*3)-1));

		auto npc = world->addEntity("Badguy " + std::to_string(x));
		npc->addComponent(new RenderComponent(0, sf::Color::Red));
		npc->addComponent(new PositionComponent(rand_x, rand_y, gl::Layer::Actor));
		npc->addComponent(new HealthComponent(10));
		npc->addComponent(new TimeComponent(70));
		npc->addComponent(new AIComponent);
		npc->addComponent(new FactionComponent(Faction::badguys));
	}

	for (int x = 0; x < 1000; ++x)
	{
		int rand_x, rand_y;

		rand_x = (rand() % ((map_chunk_size.x * 3) - 1));
		rand_y = (rand() % ((map_chunk_size.y * 3) - 1));

		auto tree = world->addEntity("Tree " + std::to_string(x));
		tree->addComponent(new RenderComponent(5, sf::Color::Green));
		tree->addComponent(new PositionComponent(rand_x, rand_y, gl::Layer::Actor));
		tree->addComponent(new HealthComponent(10));
	}


}
