#include "pch.h"
#include "MapSystem.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), map_chunk_size.x * 5, map_chunk_size.y * 5);
	world->currentMap = map.get();

	buildInitialMap();
}

void MapSystem::buildInitialMap()
{
	// Build tile layer
	for (unsigned int x = 0; x < map->getWidth(); ++x)
	{
		for (unsigned int y = 0; y < map->getHeight(); ++y)
		{
			sf::Color col;
			if (x % map_chunk_size.x && y % map_chunk_size.y)
				col = sf::Color{ 100,100,100 };
			else
				col = sf::Color{ 255,255,50 };
				
			map->placeTile(new Tile(2, col, sf::Color::Black), { x,y });
		}
	}
	// Add Player

	world->player = world->addEntity("Player");
	Entity* pl = world->player;
	pl->addComponent(new TestComponent());
	pl->addComponent(new ViewportFocusComponent);
	pl->addComponent(new RenderComponent(0, sf::Color{ 225, 225, 225 }));
	pl->addComponent(new PositionComponent(2, 16, gl::Layer::Actor));
	pl->addComponent(new HealthComponent(10));
	pl->addComponent(new TimeComponent(100));
	pl->addComponent(new PlayerAIComponent);
	pl->addComponent(new FactionComponent(Faction::goodguys));
}
