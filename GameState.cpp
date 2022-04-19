#include "pch.h"
#include "GameState.h"

void GameState::init()
{

	// add all systems before world init()

	world.addSystem(new TimeSystem);
	world.addSystem(new PlayerInputSystem);
	world.addSystem(new NPCInputSystem);
	world.addSystem(new CoordinateSystem);
	world.addSystem(new MovementSystem);
	world.addSystem(new CameraSystem);
	world.addSystem(new ViewportSystem);
	world.addSystem(new RenderSystem);
	world.addSystem(new CollisionSystem);
	world.addSystem(new GUISystem);
	// Add map system last
	world.addSystem(new MapSystem);
	

	////////////////////////////////////////////

	world.init();

	///////////////////////////////////////////
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
	world.cleanUp();
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
}

void GameState::render(sf::RenderTarget* target)
{
	world.render(target);
}
