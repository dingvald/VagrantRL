#include "pch.h"
#include "GameState.h"

void GameState::init()
{
	auto player = world.addEntity("Player");
	player->addComponent(new TestComponent("This is a test"));
	player->addComponent(new PositionComponent(10, 10));

	
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
	std::cout << dt << "\n";
	updateKeybinds(dt);
}

void GameState::render(sf::RenderTarget* target)
{
	renderer.render(target);
}
