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
	player->addComponent(new RenderComponent(3, sf::Color::White));
	player->addComponent(new PositionComponent(10, 10));
	player->addComponent(new TimeComponent(70));

	for (int i = 0; i < 1000; ++i)
	{
		auto npc = world.addEntity("NPC" + std::to_string(i));
		//npc->addComponent(new TimeComponent(10*i));
		npc->addComponent(new RenderComponent(0, sf::Color::Red));
		npc->addComponent(new PositionComponent(i, i));
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
