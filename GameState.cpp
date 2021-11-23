#include "pch.h"
#include "GameState.h"

void GameState::init()
{

	// add all systems beofre world init()

	world.addSystem(new TimeSystem);
	world.addSystem(new RenderSystem);

	////////////////////////////////////////////

	world.init();

	auto player = world.addEntity("Player");
	player->addComponent(new RenderComponent(0, sf::Color::White));
	player->addComponent(new PositionComponent(16, 16, Layer::Actor));
	player->addComponent(new TimeComponent(70));

	for (int i = 0; i < 2500; ++i)
	{
		unsigned int x, y;

		x = rand() % 1024;
		y = rand() % 640;

		auto npc = world.addEntity("NPC" + std::to_string(i));
		//npc->addComponent(new TimeComponent(10*i));
		npc->addComponent(new RenderComponent(0, sf::Color::Red));
		npc->addComponent(new PositionComponent(x, y, Layer::Actor));
	}
}

GameState::GameState()
{
	init();
}

GameState::~GameState()
{
}

void GameState::endState()
{
	std::cout << "Ending Game State." << "\n";
}

void GameState::updateKeybinds(const float dt)
{
	checkForQuit();
}

void GameState::update(const float dt)
{
	updateKeybinds(dt);
	world.update(dt);
	std::cout << "Time delta: " << dt << "\n";
}

void GameState::render(sf::RenderTarget* target)
{
	world.render(target);
}
